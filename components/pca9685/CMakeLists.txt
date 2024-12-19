idf_component_register(SRCS "pca9685_app.c"
                            "pca9685_drv.c"
                            "pca9685_intf.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES sub_bus_init esp_driver_i2c esp_driver_gpio)