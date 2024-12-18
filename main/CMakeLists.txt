idf_component_register(SRCS "navi_master.pb.c" "pb_com.c" "main.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES esp_driver_uart nanopb sub_bus_init pca9685 aht30)