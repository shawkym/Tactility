#include "CO5300Display.h"

#include <Tactility/Logger.h>

#include <esp_lcd_panel_commands.h>
#include <esp_lcd_panel_dev.h>
#include <esp_lcd_co5300.h>
#include <esp_lvgl_port.h>

static const auto LOGGER = tt::Logger("CO5300");

bool CO5300Display::createIoHandle(esp_lcd_panel_io_handle_t& outHandle) {
    LOGGER.info("Starting");

    const esp_lcd_panel_io_spi_config_t panel_io_config = {
        .cs_gpio_num = configuration->csPin,
        .dc_gpio_num = configuration->dcPin,
        .spi_mode = 0,
        .pclk_hz = configuration->pixelClockFrequency,
        .trans_queue_depth = configuration->transactionQueueDepth,
        .on_color_trans_done = nullptr,
        .user_ctx = nullptr,
        .lcd_cmd_bits = 32,
        .lcd_param_bits = 8,
        .flags = {
            .quad_mode = true,
        }
    };

    if (esp_lcd_new_panel_io_spi(configuration->spiHostDevice, &panel_io_config, &outHandle) != ESP_OK) {
        LOGGER.error("Failed to create panel");
        return false;
    }

    return true;
}

bool CO5300Display::createPanelHandle(esp_lcd_panel_io_handle_t ioHandle, esp_lcd_panel_handle_t& panelHandle) {

    const esp_lcd_panel_dev_config_t panel_config = {
        .reset_gpio_num = configuration->resetPin,
        .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
        .data_endian = LCD_RGB_DATA_ENDIAN_LITTLE,
        .bits_per_pixel = 16,
        .flags = {
            .reset_active_high = false
        },
        .vendor_config = nullptr
    };

    if (esp_lcd_new_panel_co5300(ioHandle, &panel_config, &panelHandle) != ESP_OK) {
        LOGGER.error("Failed to create panel");
        return false;
    }

    if (esp_lcd_panel_reset(panelHandle) != ESP_OK) {
        LOGGER.error("Failed to reset panel");
        return false;
    }

    if (esp_lcd_panel_init(panelHandle) != ESP_OK) {
        LOGGER.error("Failed to init panel");
        return false;
    }

    if (esp_lcd_panel_invert_color(panelHandle, configuration->invertColor) != ESP_OK) {
        LOGGER.error("Failed to set panel to invert");
        return false;
    }

    // Warning: it looks like LVGL rotation is broken when "gap" is set and the screen is moved to a non-default orientation
    int gap_x = configuration->swapXY ? configuration->gapY : configuration->gapX;
    int gap_y = configuration->swapXY ? configuration->gapX : configuration->gapY;
    if (esp_lcd_panel_set_gap(panelHandle, gap_x, gap_y) != ESP_OK) {
        LOGGER.error("Failed to set panel gap");
        return false;
    }

    if (esp_lcd_panel_swap_xy(panelHandle, configuration->swapXY) != ESP_OK) {
        LOGGER.error("Failed to swap XY ");
        return false;
    }

    if (esp_lcd_panel_mirror(panelHandle, configuration->mirrorX, configuration->mirrorY) != ESP_OK) {
        LOGGER.error("Failed to set panel to mirror");
        return false;
    }

    if (esp_lcd_panel_disp_on_off(panelHandle, true) != ESP_OK) {
        LOGGER.error("Failed to turn display on");
        return false;
    }

    return true;
}

lvgl_port_display_cfg_t CO5300Display::getLvglPortDisplayConfig(esp_lcd_panel_io_handle_t ioHandle, esp_lcd_panel_handle_t panelHandle) {
    return lvgl_port_display_cfg_t {
        .io_handle = ioHandle,
        .panel_handle = panelHandle,
        .control_handle = nullptr,
        .buffer_size = configuration->bufferSize,
        .double_buffer = false,
        .trans_size = 0,
        .hres = configuration->horizontalResolution,
        .vres = configuration->verticalResolution,
        .monochrome = false,
        .rotation = {
            .swap_xy = configuration->swapXY,
            .mirror_x = configuration->mirrorX,
            .mirror_y = configuration->mirrorY,
        },
        .color_format = LV_COLOR_FORMAT_RGB565,
        .flags = {
            .buff_dma = true,
            .buff_spiram = false,
            .sw_rotate = false,
            .swap_bytes = true,
            .full_refresh = false,
            .direct_mode = false
        }
    };
}

