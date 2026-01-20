#include "CO5300Display.h"

#include <esp_lcd_co5300.h>
#include <Tactility/Logger.h>

#include <esp_lcd_panel_dev.h>
#include <esp_lvgl_port.h>

static const char* TAG = "CO5300D";

esp_lcd_panel_io_spi_config_t CO5300Display::createPanelConfig() {
    if (configuration->use_qspi_interface)
    return CO5300_PANEL_IO_QSPI_CONFIG(configuration->csPin, NULL, NULL);
    else    
    return CO5300_PANEL_IO_SPI_CONFIG(configuration->csPin, configuration->dcPin, NULL, NULL);
    // Attach the LCD to the SPI bus;
    // return {
    //     .reset_gpio_num = Configuration->resetPin,
    //     .clkPin = Configuration->clkPin,
    //     .csPin = Configuration->csPin,
    //     .dcPin = Configuration->dcPin,
    //     .d0Pin = Configuration->d0Pin,
    //     .d1Pin = Configuration->d1Pin,
    //     .d2Pin = Configuration->d2Pin,
    //     .d3Pin = Configuration->d3Pin,
    //     .tePin = Configuration->tePin,
    //     .rgb_ele_order = Configuration->rgbElementOrder,
    //     .data_endian = LCD_RGB_DATA_ENDIAN_LITTLE,
    //     .bits_per_pixel = Configuration->bits_per_pixel,
    //     .flags = {
    //         .reset_active_high = false
    //     },
    //     .vendor_config = Configuration->vendor_config,
    //     .spiHostDevice = Configuration->spiHostDevice,
    //     .pixelClockFrequency = Configuration->pixelClockFrequency,
    //     .transactionQueueDepth = Configuration->transactionQueueDepth
    // };
}

bool CO5300Display::createPanelHandle(esp_lcd_panel_io_handle_t ioHandle, esp_lcd_panel_handle_t& panelHandle) {
    
    // if (configuration->use_qspi_interface) {
    // const spi_bus_config_t buscfg = {
    // .mosi_io_num = configuration->d0Pin,
    // .miso_io_num = configuration->d1Pin,
    // .sclk_io_num = configuration->clkPin,
    // .quadwp_io_num = configuration->d2Pin,
    // .quadhd_io_num = configuration->d3Pin,
    // };
    // //ESP_ERROR_CHECK(spi_bus_initialize(configuration->spiHostDevice, &buscfg, SPI_DMA_CH_AUTO));
    // } else {
    // const spi_bus_config_t buscfg = {
    // .mosi_io_num = configuration->d0Pin,
    // .miso_io_num = configuration->d1Pin,
    // .sclk_io_num = configuration->clkPin,
    // .quadwp_io_num = GPIO_NUM_NC,
    // .quadhd_io_num = GPIO_NUM_NC,
    // };
    // //ESP_ERROR_CHECK(spi_bus_initialize(configuration->spiHostDevice, &buscfg, SPI_DMA_CH_AUTO));
    // }

    const esp_lcd_panel_io_spi_config_t io_config = createPanelConfig();
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi((esp_lcd_spi_bus_handle_t)configuration->spiHostDevice, &io_config, &ioHandle));

    const co5300_vendor_config_t vendor_config = {
        .flags = {
            .use_qspi_interface = configuration->use_qspi_interface,
        },
    };
    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = configuration->resetPin,
        .rgb_ele_order = configuration->rgbElementOrder,
        .bits_per_pixel = configuration->bits_per_pixel,
        .vendor_config = (void *) &vendor_config,
    };
    return esp_lcd_new_panel_co5300(ioHandle, &panel_config, &panelHandle) == ESP_OK;
}
