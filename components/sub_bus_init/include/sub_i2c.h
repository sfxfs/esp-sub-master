#ifndef SUB_INTF_INIT_I2C_H
#define SUB_INTF_INIT_I2C_H

#include "esp_err.h"
#include "driver/i2c_master.h" // esp_driver_i2c

#include "sdkconfig.h"

#if CONFIG_SUB_ENABLE_I2C0
extern i2c_master_bus_handle_t i2c0_bus_handle;
#endif
#if CONFIG_SUB_ENABLE_I2C1
extern i2c_master_bus_handle_t i2c1_bus_handle;
#endif

esp_err_t sub_i2c_intf_init(void);
esp_err_t sub_i2c_intf_deinit(void);

#endif
