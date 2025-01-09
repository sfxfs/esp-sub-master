idf_component_register(SRCS
                        "main.c"
                        "navi_master.pb.c"
                        "sub_rpc.c"
                        "sub_rpc_func.c"
                        "sub_intf_init.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES
                        esp_driver_uart esp_driver_i2c esp_driver_spi # idf official components 
                        nanopb pca9685 aht30 dshot) # third-party components