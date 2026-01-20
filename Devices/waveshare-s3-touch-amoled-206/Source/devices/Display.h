#pragma once

#include <Tactility/hal/display/DisplayDevice.h>

constexpr auto AMOLED_WIDTH = 410;
constexpr auto AMOLED_HEIGHT = 502;
constexpr auto AMOLED_BUFFER_HEIGHT = AMOLED_HEIGHT / 10;
constexpr auto AMOLED_BUFFER_SIZE = AMOLED_WIDTH * AMOLED_BUFFER_HEIGHT;


std::shared_ptr<tt::hal::display::DisplayDevice> createDisplay();