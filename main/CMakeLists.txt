idf_component_register(SRCS
                        "main.c"
                    INCLUDE_DIRS "."
                    PRIV_REQUIRES
                        nanopb protobuf_rpc # sub-navi
                        sub_bus_init dshot # interface
                        pca9685 aht30) # peripheral