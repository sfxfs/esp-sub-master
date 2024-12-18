#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "pca9685_app.h"
#include "aht30_app.h"
#include "sub_intf_init.h"

#include "pb_com.h"

void app_main(void)
{
    if (0 == protobuf_commu_init())
    {
        ESP_LOGI("main", "protobuf_commu_init success");
        if (0 == protobuf_commu_start_thread())
        {
            ESP_LOGI("main", "protobuf_commu_start_thread success");
            for(;;)
            {
                vTaskDelay(1000 / portTICK_PERIOD_MS);
            }
        }
    }
}
