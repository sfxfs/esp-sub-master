#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "driver/uart.h"

#include "pb_decode.h"
#include "pb_encode.h"

#include "message_cmd.h"
#include "navi_master.pb.h"

#include "sdkconfig.h"

#include "pb_rpc.h"

static const char *TAG = "protobuf_commu";

static QueueHandle_t uart_queue;

static const pb_msgdesc_t *decode_command_unionmessage_type(pb_istream_t *stream)
{
    pb_wire_type_t wire_type;
    uint32_t tag;
    bool eof;

    while (pb_decode_tag(stream, &wire_type, &tag, &eof))
    {
        if (wire_type == PB_WT_STRING)
        {
            pb_field_iter_t iter;
            if (pb_field_iter_begin(&iter, Commands_fields, NULL) &&
                pb_field_iter_find(&iter, tag))
            {
                /* Found our field. */
                return iter.submsg_desc;
            }
        }

        /* Wasn't our field.. */
        pb_skip_field(stream, wire_type);
    }

    return NULL;
}

static bool decode_unionmessage_contents(pb_istream_t *stream, const pb_msgdesc_t *messagetype, void *dest_struct)
{
    pb_istream_t substream;
    bool status;
    if (!pb_make_string_substream(stream, &substream))
        return false;

    status = pb_decode(&substream, messagetype, dest_struct);
    pb_close_string_substream(stream, &substream);
    return status;
}

static int protobuf_command_rpc(uint8_t *data, size_t size)
{
    pb_istream_t stream = pb_istream_from_buffer(data, size);

    const pb_msgdesc_t *type = decode_command_unionmessage_type(&stream);
    bool status = false;

    if (type == ThrusterCommand_fields)
    {
        ThrusterCommand msg = {};
        status = decode_unionmessage_contents(&stream, ThrusterCommand_fields, &msg);
        ESP_LOGD(TAG, "Got ThrusterCommand");
        message_thruster_cmd(&msg);
    }
    else if (type == PWMDevCommand_fields)
    {
        PWMDevCommand msg = {};
        status = decode_unionmessage_contents(&stream, PWMDevCommand_fields, &msg);
        ESP_LOGD(TAG, "Got ArmCommand");
        message_pwmDev_cmd(&msg);
    }

    if (!status)
    {
        ESP_LOGW(TAG, "Decode %d bytes data failed: %s", size, PB_GET_ERROR(&stream));
        return -1;
    }

    return 0;
}

int protobuf_commu_init(void)
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
        return -1;
#if CONFIG_SUB_PROTOBUF_UART_CUSTOM_PINS
    if (ESP_OK != uart_set_pin(CONFIG_SUB_PROTOBUF_UART_PORT, CONFIG_SUB_PROTOBUF_UART_TX_PIN, CONFIG_SUB_PROTOBUF_UART_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE))
        return -1;
#endif
    if (uart_driver_install(CONFIG_SUB_PROTOBUF_UART_PORT,
                            NAVI_MASTER_PB_H_MAX_SIZE * 2, NAVI_MASTER_PB_H_MAX_SIZE * 2,
                            CONFIG_SUB_PROTOBUF_UART_QUEUE_SIZE, &uart_queue, 0) != ESP_OK)
    {
        ESP_LOGE(TAG, "Driver installation failed");
        return -1;
    }
    return 0;
}

static void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    uint8_t* dtmp = (uint8_t*) malloc(NAVI_MASTER_PB_H_MAX_SIZE);
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

int protobuf_commu_start_thread(void)
{
    if (pdPASS == xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL))
        return 0;
    return -1;
}

static bool encode_unionmessage_resp(pb_ostream_t *stream, const pb_msgdesc_t *messagetype, void *message)
{
    pb_field_iter_t iter;

    if (!pb_field_iter_begin(&iter, Responses_fields, message))
        return false;

    do
    {
        if (iter.submsg_desc == messagetype)
        {
            /* This is our field, encode the message using it. */
            if (!pb_encode_tag_for_field(stream, &iter))
                return false;

            return pb_encode_submessage(stream, messagetype, message);
        }
    } while (pb_field_iter_next(&iter));

    /* Didn't find the field for messagetype */
    return false;
}

int protobuf_commu_send_resp(const pb_msgdesc_t *messagetype, void *message)
{
    uint8_t data[NAVI_MASTER_PB_H_MAX_SIZE];
    pb_ostream_t stream = pb_ostream_from_buffer(data, sizeof(data));

    bool status = encode_unionmessage_resp(&stream, messagetype, &message);
    if (!status)
    {
        ESP_LOGW(TAG, "encoding failed");
        return -1;
    }

    if (uart_write_bytes(CONFIG_SUB_PROTOBUF_UART_PORT, data, stream.bytes_written) < 0)
    {
        ESP_LOGW(TAG, "protobuf uart write failed");
        return -1;
    }
    ESP_LOGD(TAG, "protobuf uart write success");
    return 0;
}
