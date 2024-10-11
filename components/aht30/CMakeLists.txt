idf_component_register(SRCS "aht30_app.c"
                    "aht30_drv.c"
                    "aht30_intf.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES esp_driver_i2c)