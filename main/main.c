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
    if (ESP_OK != sub_bus_intf_init())
    {
        for (;;)
        {
            ESP_LOGE(TAG, "sub_all_intf_init failed");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
    ESP_LOGI(TAG, "sub_all_intf_init successed");

    // protobuf通信用到的外设初始化
    if (ESP_OK != sub_rpc_handle_func_init())
    {
        for (;;)
        {
            ESP_LOGE(TAG, "sub_rpc_handle_func_init failed");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
    ESP_LOGI(TAG, "sub_rpc_handle_func_init successed");

    // protobuf通信初始化
    if (ESP_OK != sub_rpc_init())
    {
        for (;;)
        {
            ESP_LOGE(TAG, "sub_rpc_init failed");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
    ESP_LOGI(TAG, "sub_rpc_init successed");

    // protobuf通信线程启动
    if (ESP_OK != sub_rpc_start_thread())
    {
        for (;;)
        {
            ESP_LOGE(TAG, "sub_rpc_start_thread failed");
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
    ESP_LOGI(TAG, "sub_rpc_start_thread successed");

    // 无限循环，发送心跳包
    for (;;)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        // heartbeat packet TODO ...
    }
}
