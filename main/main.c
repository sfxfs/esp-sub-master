#include <stdio.h>

#include "driver/i2c_master.h" // esp_driver_i2c
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "pca9685_app.h"
#include "aht30_app.h"

i2c_master_bus_handle_t i2c0_bus_handle;
i2c_master_bus_handle_t i2c1_bus_handle;

int i2c_all_bus_init()
{
    i2c_master_bus_config_t i2c_bus_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = 0,
        .scl_io_num = 12,
        .sda_io_num = 13,
        .glitch_ignore_cnt = 7,
    };
    if (ESP_OK != i2c_new_master_bus(&i2c_bus_config, &i2c0_bus_handle))
        return 1;

    i2c_bus_config.i2c_port = 1;
    i2c_bus_config.scl_io_num = 14;
    i2c_bus_config.sda_io_num = 15;
    if (ESP_OK != i2c_new_master_bus(&i2c_bus_config, &i2c1_bus_handle))
        return 1;

    return 0;
}

void app_main(void)
{
    if (0 == i2c_all_bus_init())
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
