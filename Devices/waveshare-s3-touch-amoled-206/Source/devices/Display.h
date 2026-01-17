#pragma once

#include <Tactility/hal/display/DisplayDevice.h>
#include <driver/gpio.h>
#include <driver/spi_common.h>
#include <memory>

constexpr auto LCD_HORIZONTAL_RESOLUTION = 410;
constexpr auto LCD_VERTICAL_RESOLUTION = 502;
constexpr auto LCD_BUFFER_HEIGHT = LCD_VERTICAL_RESOLUTION / 10;
constexpr auto LCD_BUFFER_SIZE = LCD_HORIZONTAL_RESOLUTION * LCD_BUFFER_HEIGHT;


std::shared_ptr<tt::hal::display::DisplayDevice> createDisplay();