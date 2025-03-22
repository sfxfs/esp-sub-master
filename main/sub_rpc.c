#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "driver/uart.h"

#include "pb_decode.h"
#include "pb_encode.h"

#include "pb_unimsg.h"
#include "navi_master.pb.h"

#include "sub_rpc_func.h"

#include "sdkconfig.h"

#include "sub_rpc.h"

static const char *TAG = "sub_rpc";

static QueueHandle_t uart_queue;

static int protobuf_command_rpc(uint8_t *data, size_t size)
{
    pb_istream_t stream = pb_istream_from_buffer(data, size);

    const pb_msgdesc_t *type = decode_command_unionmessage_type(&stream);
    bool status = false;

    if (type == ThrusterCommand_fields)
    {
        ThrusterCommand msg = {};
        status = decode_unionmessage_contents(&stream, ThrusterCommand_fields, &msg);
        ESP_LOGI(TAG, "Got ThrusterCommand");
        handle_message_thruster_cmd(&msg);
    }
    else if (type == PWMDevCommand_fields)
    {
        PWMDevCommand msg = {};
        status = decode_unionmessage_contents(&stream, PWMDevCommand_fields, &msg);
        ESP_LOGI(TAG, "Got ArmCommand");
        handle_message_pwmDev_cmd(&msg);
    }

    if (!status)
    {
        ESP_LOGW(TAG, "Decode %d bytes data failed: %s", size, PB_GET_ERROR(&stream));
        return -1;
    }

    return 0;
}

esp_err_t sub_rpc_init(void)
{
    uart_config_t uart_config = {
        .baud_rate = CONFIG_SUB_PROTOBUF_UART_BAUDRATE,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };
    if (ESP_OK != uart_param_config(CONFIG_SUB_PROTOBUF_UART_PORT, &uart_config))
    {
        ESP_LOGE(TAG, "UART config failed");
        return ESP_FAIL;
    }
#if CONFIG_SUB_PROTOBUF_UART_CUSTOM_PINS
    if (ESP_OK != uart_set_pin(CONFIG_SUB_PROTOBUF_UART_PORT,
                               CONFIG_SUB_PROTOBUF_UART_TX_PIN,
                               CONFIG_SUB_PROTOBUF_UART_RX_PIN,
                               UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE))
    {
        ESP_LOGE(TAG, "UART set pin failed");
        return ESP_FAIL;
    }
#endif
    if (uart_driver_install(CONFIG_SUB_PROTOBUF_UART_PORT,
                            NAVI_MASTER_PB_H_MAX_SIZE * 2,
                            NAVI_MASTER_PB_H_MAX_SIZE * 2,
                            CONFIG_SUB_PROTOBUF_UART_QUEUE_SIZE,
                            &uart_queue, 0) != ESP_OK)
    {
        ESP_LOGE(TAG, "UART driver installation failed");
        return ESP_FAIL;
    }
    ESP_LOGI(TAG, "UART driver installed");
    return ESP_OK;
}

static void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    uint8_t *dtmp = (uint8_t *)malloc(NAVI_MASTER_PB_H_MAX_SIZE);
    for (;;)
    {
        // Waiting for UART event.
        if (xQueueReceive(uart_queue, (void *)&event, (TickType_t)portMAX_DELAY))
        {
            ESP_LOGD(TAG, "protobuf uart event");
            switch (event.type)
            {
            // Event of UART receving data
            /*We'd better handler data event fast, there would be much more data events than
            other types of events. If we take too much time on data event, the queue might
            be full.*/
            case UART_DATA:
                ESP_LOGD(TAG, "[UART DATA]: %d", event.size);
                int len = uart_read_bytes(CONFIG_SUB_PROTOBUF_UART_PORT, dtmp, event.size, portMAX_DELAY);
                // proccess data
                protobuf_command_rpc(dtmp, len);
                break;
            // Event of HW FIFO overflow detected
            case UART_FIFO_OVF:
                ESP_LOGW(TAG, "hw fifo overflow");
                // If fifo overflow happened, you should consider adding flow control for your application.
                // The ISR has already reset the rx FIFO,
                // As an example, we directly flush the rx buffer here in order to read more data.
                uart_flush_input(CONFIG_SUB_PROTOBUF_UART_PORT);
                xQueueReset(uart_queue);
                break;
            // Event of UART ring buffer full
            case UART_BUFFER_FULL:
                ESP_LOGW(TAG, "ring buffer full");
                // If buffer full happened, you should consider increasing your buffer size
                // As an example, we directly flush the rx buffer here in order to read more data.
                uart_flush_input(CONFIG_SUB_PROTOBUF_UART_PORT);
                xQueueReset(uart_queue);
                break;
            // Event of UART RX break detected
            case UART_BREAK:
                ESP_LOGD(TAG, "uart rx break");
                break;
            // Event of UART parity check error
            case UART_PARITY_ERR:
                ESP_LOGW(TAG, "uart parity error");
                break;
            // Event of UART frame error
            case UART_FRAME_ERR:
                ESP_LOGW(TAG, "uart frame error");
                break;
            // Others
            default:
                ESP_LOGD(TAG, "uart event type: %d", event.type);
                break;
            }
        }
    }
    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}

esp_err_t sub_rpc_start_thread(void)
{
    if (pdPASS == xTaskCreate(uart_event_task, "uart_event_task", 4096, NULL, 12, NULL))
        return ESP_OK;
    return ESP_FAIL;
}

esp_err_t sub_rpc_send_resp(const pb_msgdesc_t *messagetype, void *message)
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
