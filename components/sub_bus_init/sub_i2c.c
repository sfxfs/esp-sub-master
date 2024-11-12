#include "driver/i2c_master.h" // esp_driver_i2c
#include "sdkconfig.h"

#include "sub_i2c.h"

#if CONFIG_SUB_ENABLE_I2C0
i2c_master_bus_handle_t i2c0_bus_handle;
#endif
#if CONFIG_SUB_ENABLE_I2C1
i2c_master_bus_handle_t i2c1_bus_handle;
#endif

esp_err_t sub_i2c_intf_init()
{
    esp_err_t ret = ESP_OK;
#if CONFIG_SUB_ENABLE_I2C0
{
    i2c_master_bus_config_t i2c_bus_config = {
        .i2c_port = 0,
        .sda_io_num = CONFIG_SUB_I2C0_SDA_PIN,
        .scl_io_num = CONFIG_SUB_I2C0_SCL_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
    };
    ret += i2c_new_master_bus(&i2c_bus_config, &i2c0_bus_handle);
}
#endif
#if CONFIG_SUB_ENABLE_I2C1
{
    i2c_master_bus_config_t i2c_bus_config = {
        .i2c_port = 1,
        .sda_io_num = CONFIG_SUB_I2C1_SDA_PIN,
        .scl_io_num = CONFIG_SUB_I2C1_SCL_PIN,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
    };
    ret += i2c_new_master_bus(&i2c_bus_config, &i2c1_bus_handle);
}
#endif
    return ret;
}

esp_err_t sub_i2c_intf_deinit()
{
    esp_err_t ret = ESP_OK;
#if CONFIG_SUB_ENABLE_I2C0
    ret += i2c_del_master_bus(i2c0_bus_handle);
#endif
#if CONFIG_SUB_ENABLE_I2C1
    ret += i2c_del_master_bus(i2c1_bus_handle);
#endif
    return ret;
}
