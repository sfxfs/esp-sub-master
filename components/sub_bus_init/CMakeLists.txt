idf_component_register(SRCS "sub_intf_init.c"
                            "sub_i2c.c"
                            "sub_spi.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES esp_driver_i2c esp_driver_spi)