// stdlibs
#include <stdio.h>

// offical components
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// main
#include "sub_rpc.h"
#include "sub_rpc_func.h"
#include "sub_intf_init.h"

static const char *TAG = "main";

void app_main(void)
{
    // 各总线端口初始化 spi iic
    if (0 != sub_bus_intf_init())
    {
        for (;;)
        {
            ESP_LOGE(TAG, "sub_all_intf_init failed");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }

    // protobuf通信用到的外设初始化
    if (0 != sub_rpc_handle_func_init())
    {
        for (;;)
        {
            ESP_LOGE(TAG, "message_cmd_init failed");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }

    // protobuf通信初始化
    if (0 != sub_rpc_init())
    {
        for (;;)
        {
            ESP_LOGE(TAG, "protobuf_commu_init failed");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }

    // protobuf通信线程启动
    if (0 != sub_rpc_start_thread())
    {
        for (;;)
        {
            ESP_LOGE(TAG, "protobuf_commu_start_thread failed");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }

    // 无限循环，发送心跳包
    for (;;)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // heartbeat packet TODO ...
    }
}
