#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#include "esp_log.h"
#include "driver/i2c_master.h"
#include "driver/uart.h"

#include "wit_c_sdk.h"
#include "REG.h"

#include "pb_encode.h"
#include "pb_unimsg.h"
#include "navi_master.pb.h"

#include "sdkconfig.h"

#include "jy901.h"

static const char *TAG = "jy901";

static i2c_master_dev_handle_t jy901_i2c_dev_handle;

static TimerHandle_t jy901_timer;

static wt_jy901_data_t g_jy901_data; // global jy901 data

static void jy901_data_update(uint32_t uiReg, uint32_t uiRegNum)
{
    ESP_LOGD(TAG, "jy901_data_update triggered!");
    for (int i = 0; i < uiRegNum; i++)
    {
        switch (uiReg)
        {
        case AX:
            g_jy901_data.Acc[0] = sReg[AX] / 32768.0f * 16.0f;
            break;
        case AY:
            g_jy901_data.Acc[1] = sReg[AY] / 32768.0f * 16.0f;
            break;
        case AZ:
            g_jy901_data.Acc[2] = sReg[AZ] / 32768.0f * 16.0f;
            break;
        case GX:
            g_jy901_data.Gyro[0] = sReg[GX] / 32768.0f * 2000.0f;
            break;
        case GY:
            g_jy901_data.Gyro[1] = sReg[GY] / 32768.0f * 2000.0f;
            break;
        case GZ:
            g_jy901_data.Gyro[2] = sReg[GZ] / 32768.0f * 2000.0f;
            break;
        case HX:
            break;
        case HY:
            break;
        case HZ:
            break;
        case Roll:
            g_jy901_data.Angle[0] = sReg[Roll] / 32768.0f * 180.0f;
            break;
        case Pitch:
            g_jy901_data.Angle[1] = sReg[Pitch] / 32768.0f * 180.0f;
            break;
        case Yaw:
            g_jy901_data.Angle[2] = sReg[Yaw] / 32768.0f * 180.0f;
            break;
        default:
            break;
        }
        uiReg++;
    }
}

static void jy901_delay_ms(uint16_t ucMs)
{
    vTaskDelay(ucMs / portTICK_PERIOD_MS);
}

static int32_t jy901_i2c_read(uint8_t ucAddr, uint8_t ucReg, uint8_t *p_ucVal, uint32_t uiLen)
{
    if (ESP_OK == i2c_master_transmit_receive(jy901_i2c_dev_handle, &ucReg, 1, p_ucVal, uiLen, CONFIG_SUB_JY901_IIC_TIMEOUT))
        return 1;   // 1 stands for success
    else
        return 0;
}

static int32_t jy901_i2c_write(uint8_t ucAddr, uint8_t ucReg, uint8_t *p_ucVal, uint32_t uiLen)
{
    uint8_t *temp_buf = malloc(uiLen + 1);
    if (NULL == temp_buf)
        return 1;

    temp_buf[0] = ucReg;
    memcpy(&temp_buf[1], p_ucVal, uiLen);

    esp_err_t ret = i2c_master_transmit(jy901_i2c_dev_handle, temp_buf, uiLen + 1, CONFIG_SUB_JY901_IIC_TIMEOUT);

    free(temp_buf);

    if (ESP_OK == ret)
        return 1;      // 1 stands for success
    else
        return 0;
}

esp_err_t jy901_init(void)
{
    i2c_master_bus_handle_t jy901_i2c_handle;
    if (ESP_OK != i2c_master_get_bus_handle(CONFIG_SUB_JY901_IIC_PORT, &jy901_i2c_handle))
    {
        ESP_LOGE(TAG, "get i2c%d bus handle failed", CONFIG_SUB_JY901_IIC_PORT);
        return ESP_FAIL;
    }
    i2c_device_config_t i2c_dev_conf = {
        .device_address = CONFIG_SUB_JY901_IIC_ADDRESS,
        .scl_speed_hz = CONFIG_SUB_JY901_IIC_FREQUENCY,
    };
    if (ESP_OK != i2c_master_bus_add_device(jy901_i2c_handle, &i2c_dev_conf, &jy901_i2c_dev_handle))
    {
        ESP_LOGE(TAG, "add device on i2c%d failed", CONFIG_SUB_JY901_IIC_PORT);
        return ESP_FAIL;
    }
    // wit sdk init
    if (WIT_HAL_OK != WitInit(WIT_PROTOCOL_I2C, CONFIG_SUB_JY901_IIC_ADDRESS)) // 0x50 is 7 bit iic address of jy901
    {
        ESP_LOGE(TAG, "wit sdk init failed");
        return ESP_FAIL;
    }
    WitDelayMsRegister(jy901_delay_ms);
    WitRegisterCallBack(jy901_data_update);
    WitI2cFuncRegister(jy901_i2c_write, jy901_i2c_read); // register iic read/write callback
    return ESP_OK;
}

static esp_err_t sub_rpc_send_resp(const pb_msgdesc_t *messagetype, void *message)
{
    uint8_t data[NAVI_MASTER_PB_H_MAX_SIZE];
    pb_ostream_t stream = pb_ostream_from_buffer(data, sizeof(data));

    bool status = encode_response_unionmessage(&stream, messagetype, message);
    if (!status)
    {
        ESP_LOGW(TAG, "encoding failed");
        return ESP_FAIL;
    }

    if (uart_write_bytes(CONFIG_SUB_PROTOBUF_UART_PORT, data, stream.bytes_written) < 0)
    {
        ESP_LOGW(TAG, "protobuf UART write failed");
        return ESP_FAIL;
    }
    ESP_LOGD(TAG, "protobuf UART write success");
    return ESP_OK;
}

static void vTimerCallback(TimerHandle_t xTimer)
{
    static int error_count = 0;
    if (error_count > 10)
    {
        ESP_LOGE(TAG, "jy901 read error count exceed 10, stop reading");
        xTimerStop(jy901_timer, 0);
    }
    if (WIT_HAL_OK != WitReadReg(Roll, 3))
    {
        ESP_LOGE(TAG, "read jy901 data failed");
        error_count++;
    }
    else
    {
        // send data to main MPU ...
        MotionSensorResponse msg = {
            .roll = g_jy901_data.Angle[0],
            .pitch = g_jy901_data.Angle[1],
            .yaw = g_jy901_data.Angle[2],
        };
        // ESP_LOGI(TAG, "roll: %f, pitch: %f, yaw: %f", msg.roll, msg.pitch, msg.yaw);    // delete this line
        if (ESP_OK != sub_rpc_send_resp(MotionSensorResponse_fields, &msg))
        {
            ESP_LOGE(TAG, "send response failed");
        }
    }
}

esp_err_t jy901_routine_start(void)
{
    // if we have stack overflow issue, we can try to increase the stack size of the timer task
    jy901_timer = xTimerCreate("jy901_routine", pdMS_TO_TICKS(CONFIG_SUB_JY901_ROUTINE_PER_MS), pdTRUE, (void*)0, vTimerCallback);
    if (jy901_timer == NULL)
    {
        ESP_LOGE(TAG, "create jy901 routine timer failed");
        return ESP_FAIL;
    }
    xTimerStart(jy901_timer, 0);
    return ESP_OK;
}
