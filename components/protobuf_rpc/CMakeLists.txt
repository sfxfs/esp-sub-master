idf_component_register(SRCS "pb_rpc.c" "message_cmd.c" "navi_master.pb.c"
                    INCLUDE_DIRS "include"
                    PRIV_REQUIRES esp_driver_uart
                                    dshot pca9685
                    REQUIRES nanopb)
