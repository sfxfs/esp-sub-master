idf_component_register(SRCS "sub_intf_init.c"
                    "sub_i2c.c"
                    "sub_spi.c"
                    "sub_uart.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES esp_driver_i2c
                    esp_driver_spi
                    esp_driver_uart)