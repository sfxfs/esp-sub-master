/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_aht30_interface_template.c
 * @brief     driver aht30 interface template source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2023-11-30
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2023/11/30  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "driver/i2c_master.h" // esp_driver_i2c

#include "sdkconfig.h"

#include "aht30_intf.h"

#if CONFIG_SUB_ENABLE_AHT30

    #if (CONFIG_SUB_AHT30_IIC_PORT == 0) && CONFIG_SUB_ENABLE_I2C0
    extern i2c_master_bus_handle_t i2c0_bus_handle;
    #define AHT30_I2C_BUS i2c0_bus_handle
    #elif (CONFIG_SUB_AHT30_IIC_PORT == 1) && CONFIG_SUB_ENABLE_I2C0
    extern i2c_master_bus_handle_t i2c1_bus_handle;
    #define AHT30_I2C_BUS i2c1_bus_handle
    #else
    #error certain i2c num not found or disabled
    #endif

#endif

i2c_master_dev_handle_t aht30_i2c_dev_handle;

/**
 * @brief  interface iic bus init
 * @return status code
 *         - 0 success
 *         - 1 iic init failed
 * @note   none
 */
uint8_t aht30_interface_iic_init(void)
{
#if CONFIG_SUB_ENABLE_AHT30
    i2c_device_config_t i2c_dev_conf = {
        .device_address = CONFIG_SUB_AHT30_IIC_ADDRESS,
        .scl_speed_hz = CONFIG_SUB_AHT30_IIC_FREQUENCY,
    };
    if (ESP_OK == i2c_master_bus_add_device(AHT30_I2C_BUS, &i2c_dev_conf, &aht30_i2c_dev_handle))
        return 0;
    else
        return 1;   // NO_MEM
#else
    return 1;   // disabled
#endif
}

/**
 * @brief  interface iic bus deinit
 * @return status code
 *         - 0 success
 *         - 1 iic deinit failed
 * @note   none
 */
uint8_t aht30_interface_iic_deinit(void)
{
#if CONFIG_SUB_ENABLE_AHT30
    if (ESP_OK == i2c_master_bus_rm_device(aht30_i2c_dev_handle))
        return 0;
    else
        return 1;
#else
    return 1;   // disabled
#endif
}

/**
 * @brief      interface iic bus read
 * @param[in]  addr is the iic device write address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t aht30_interface_iic_read_cmd(uint8_t addr, uint8_t *buf, uint16_t len)
{
#if CONFIG_SUB_ENABLE_AHT30
    if (ESP_OK == i2c_master_receive(aht30_i2c_dev_handle, buf, len, CONFIG_SUB_AHT30_IIC_TIMEOUT))
        return 0;
    else
        return 1;
#else
    return 1;   // disabled
#endif
}

/**
 * @brief     interface iic bus write
 * @param[in] addr is the iic device write address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t aht30_interface_iic_write_cmd(uint8_t addr, uint8_t *buf, uint16_t len)
{
#if CONFIG_SUB_ENABLE_AHT30
    if (ESP_OK == i2c_master_transmit(aht30_i2c_dev_handle, buf, len, CONFIG_SUB_AHT30_IIC_TIMEOUT))
        return 0;
    else
        return 1;
#else
    return 1;   // disabled
#endif
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void aht30_interface_delay_ms(uint32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void aht30_interface_debug_print(const char *const fmt, ...)
{
    va_list list;
    va_start(list, fmt);
    esp_log_writev(ESP_LOG_DEBUG, "sub_aht30", fmt, list);
    va_end(list);
}
