#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "pca9685_app.h"
#include "aht30_app.h"
#include "sub_intf_init.h"


void app_main(void)
{
    if (0 == sub_all_intf_init())
    {
        if (0 == pca9685_app_init(PCA9685_ADDRESS_A000000, 50))
        {
            pca9685_app_write(PCA9685_CHANNEL_0, 0.0f, 50.0f);
            pca9685_app_write(PCA9685_CHANNEL_1, 0.0f, 50.0f);
            pca9685_app_write(PCA9685_CHANNEL_2, 0.0f, 50.0f);
            pca9685_app_write(PCA9685_CHANNEL_3, 0.0f, 50.0f);
            pca9685_app_write(PCA9685_CHANNEL_4, 0.0f, 50.0f);
            pca9685_app_write(PCA9685_CHANNEL_5, 0.0f, 50.0f);
            pca9685_app_write(PCA9685_CHANNEL_6, 0.0f, 50.0f);
            pca9685_app_write(PCA9685_CHANNEL_7, 0.0f, 50.0f);

            pca9685_app_write(PCA9685_CHANNEL_8, 0.0f, 20.0f);
            pca9685_app_write(PCA9685_CHANNEL_9, 0.0f, 20.0f);
            pca9685_app_write(PCA9685_CHANNEL_10, 0.0f, 20.0f);
            pca9685_app_write(PCA9685_CHANNEL_11, 0.0f, 20.0f);
            pca9685_app_write(PCA9685_CHANNEL_12, 0.0f, 20.0f);
            pca9685_app_write(PCA9685_CHANNEL_13, 0.0f, 20.0f);
            pca9685_app_write(PCA9685_CHANNEL_14, 0.0f, 20.0f);
            pca9685_app_write(PCA9685_CHANNEL_15, 0.0f, 20.0f);
        }
        if (0 == aht30_app_init())
        {
            float temp;
            uint8_t humi;
            for (;;)
            {
                vTaskDelay(pdMS_TO_TICKS(500));
                aht30_app_read(&temp, &humi);
                printf("temp: %f humi: %d\n", temp, (int)humi);
            }
        }
    }
}
