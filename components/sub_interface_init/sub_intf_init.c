#include "sub_intf_init.h"
#include "sub_i2c.h"

esp_err_t sub_all_intf_init(void)
{
    esp_err_t ret = ESP_OK;
    ret += sub_i2c_intf_init();
    return ret;
}

esp_err_t sub_all_intf_deinit(void)
{
    esp_err_t ret = ESP_OK;
    ret += sub_i2c_intf_deinit();
    return ret;
}
