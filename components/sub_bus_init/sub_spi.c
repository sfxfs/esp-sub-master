#include "driver/spi_master.h" // esp_driver_i2c
#include "sdkconfig.h"

#include "sub_spi.h"

esp_err_t sub_spi_intf_init(void)
{
    esp_err_t ret = ESP_OK;
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

esp_err_t sub_spi_intf_deinit(void)
{
    esp_err_t ret = ESP_OK;
#if CONFIG_SUB_ENABLE_SPI2
    ret += spi_bus_free(SPI2_HOST);
#endif
#if CONFIG_SUB_ENABLE_SPI3
    ret += spi_bus_free(SPI3_HOST);
#endif
    return ret;
}
