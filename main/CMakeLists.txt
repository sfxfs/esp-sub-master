idf_component_register(SRCS
                        "main.c"
                        "sub_rpc.c"
                        "sub_rpc_func.c"
                        "sub_intf_init.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES
                        esp_driver_uart esp_driver_i2c esp_driver_spi esp_driver_ledc esp_driver_gpio # idf official components 
                        pb_unimsg pca9685 aht30 dshot wit_sdk jy901) # third-party components