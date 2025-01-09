#include "driver/spi_master.h"
#include "driver/i2c_master.h"

#include "sdkconfig.h"

#include "sub_intf_init.h"

int sub_bus_intf_init(void)
{
    int ret = ESP_OK;
// IIC ------------------------
#if CONFIG_SUB_ENABLE_I2C0
{
    i2c_master_bus_handle_t i2c0_bus_handle;
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
    i2c_master_bus_handle_t i2c1_bus_handle;
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

// SPI ------------------------
#if CONFIG_SUB_ENABLE_SPI2
{
    spi_bus_config_t buscfg = {
        .miso_io_num = CONFIG_SUB_SPI2_MISO_PIN,
        .mosi_io_num = CONFIG_SUB_SPI2_MOSI_PIN,
        .sclk_io_num = CONFIG_SUB_SPI2_CLK_PIN,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = SOC_SPI_MAXIMUM_BUFFER_SIZE,
    };
    ret += spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
}
#endif
#if CONFIG_SUB_ENABLE_SPI3
{
    spi_bus_config_t buscfg = {
        .miso_io_num = CONFIG_SUB_SPI3_MISO_PIN,
        .mosi_io_num = CONFIG_SUB_SPI3_MOSI_PIN,
        .sclk_io_num = CONFIG_SUB_SPI3_CLK_PIN,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = SOC_SPI_MAXIMUM_BUFFER_SIZE,
    };
    ret += spi_bus_initialize(SPI3_HOST, &buscfg, SPI_DMA_CH_AUTO);
}
#endif
    return ret;
}
