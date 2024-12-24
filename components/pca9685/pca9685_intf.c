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
 * @file      driver_pca9685_interface_template.c
 * @brief     driver pca9685 interface template source file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-02-20
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/02/20  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_log.h"
#include "driver/gpio.h"
#include "driver/i2c_master.h"

#include "sdkconfig.h"

#include "pca9685_intf.h"

i2c_master_dev_handle_t pca9685_i2c_dev_handle;

/**
 * @brief  interface iic bus init
 * @return status code
 *         - 0 success
 *         - 1 iic init failed
 * @note   none
 */
uint8_t pca9685_interface_iic_init(void)
{
#if CONFIG_SUB_ENABLE_PCA9685
    esp_err_t handle_ret;
    i2c_master_bus_handle_t pca9685_i2c_handle;
    #if (CONFIG_SUB_PCA9685_IIC_PORT == 0) && CONFIG_SUB_ENABLE_I2C0
    handle_ret = i2c_master_get_bus_handle(0, &pca9685_i2c_handle);
    #elif (CONFIG_SUB_PCA9685_IIC_PORT == 1) && CONFIG_SUB_ENABLE_I2C1
    handle_ret = i2c_master_get_bus_handle(1, &pca9685_i2c_handle);
    #else
    #error certain i2c num not found or disabled
    #endif
    if (ESP_OK != handle_ret)
        return 1;   // interface not init

    i2c_device_config_t i2c_dev_conf = {
        .device_address = CONFIG_SUB_PCA9685_IIC_ADDRESS,
        .scl_speed_hz = CONFIG_SUB_PCA9685_IIC_FREQUENCY,
    };
    if (ESP_OK == i2c_master_bus_add_device(pca9685_i2c_handle, &i2c_dev_conf, &pca9685_i2c_dev_handle))
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
uint8_t pca9685_interface_iic_deinit(void)
{
#if CONFIG_SUB_ENABLE_PCA9685
    if (ESP_OK == i2c_master_bus_rm_device(pca9685_i2c_dev_handle))
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
 * @param[in] reg is the iic register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of the data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t pca9685_interface_iic_write(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
#if CONFIG_SUB_ENABLE_PCA9685
    uint8_t *temp_buf = malloc(len + 1);
    if (NULL == temp_buf)
        return 1;

    temp_buf[0] = addr;
    memcpy(&temp_buf[1], buf, len);

    esp_err_t ret = i2c_master_transmit(pca9685_i2c_dev_handle, temp_buf, len + 1, CONFIG_SUB_PCA9685_IIC_TIMEOUT);

    free(temp_buf);

    if (ESP_OK == ret)
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
 * @param[in]  reg is the iic register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t pca9685_interface_iic_read(uint8_t addr, uint8_t reg, uint8_t *buf, uint16_t len)
{
#if CONFIG_SUB_ENABLE_PCA9685
    if (ESP_OK == i2c_master_transmit_receive(pca9685_i2c_dev_handle, &reg, 1, buf, len, CONFIG_SUB_PCA9685_IIC_TIMEOUT))
        return 0;
    else
        return 1;
#else
    return 1;   // disabled
#endif
}

/**
 * @brief  interface oe init
 * @return status code
 *         - 0 success
 *         - 1 clock init failed
 * @note   none
 */
uint8_t pca9685_interface_oe_init(void)
{
#if CONFIG_SUB_ENABLE_PCA9685 && CONFIG_SUB_PCA9685_OE_GPIO_ENABLE
    if (GPIO_IS_VALID_OUTPUT_GPIO(CONFIG_SUB_PCA9685_OE_GPIO_NUM) == false)
        return 1;

    gpio_config_t io_conf = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL<<CONFIG_SUB_PCA9685_OE_GPIO_NUM),
        .pull_down_en = 0,
        .pull_up_en = 0,
    };
    if (ESP_OK == gpio_config(&io_conf))
        return 0;
    else
        return 1;
#else
    return 0;   // disabled
#endif
}

/**
 * @brief  interface oe deinit
 * @return status code
 *         - 0 success
 *         - 1 clock deinit failed
 * @note   none
 */
uint8_t pca9685_interface_oe_deinit(void)
{
#if CONFIG_SUB_ENABLE_PCA9685 && CONFIG_SUB_PCA9685_OE_GPIO_ENABLE
    if (ESP_OK == gpio_reset_pin(CONFIG_SUB_PCA9685_OE_GPIO_NUM))
        return 0;
    else
        return 1;
#else
    return 0;   // disabled
#endif
}

/**
 * @brief     interface oe write
 * @param[in] value is the written value
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t pca9685_interface_oe_write(uint8_t value)
{
#if CONFIG_SUB_ENABLE_PCA9685 && CONFIG_SUB_PCA9685_OE_GPIO_ENABLE
    if (ESP_OK == gpio_set_level(CONFIG_SUB_PCA9685_OE_GPIO_NUM, value))
        return 0;
    else
        return 1;
#else
    return 0;   // disabled
#endif
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void pca9685_interface_delay_ms(uint32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void pca9685_interface_debug_print(const char *const fmt, ...)
{
    va_list list;
    va_start(list, fmt);
    esp_log_writev(ESP_LOG_DEBUG, "sub_pca9685", fmt, list);
    va_end(list);
}
