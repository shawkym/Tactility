#include "Display.h"

#include <Ft6x36Touch.h>
#include <PwmBacklight.h>
#include <CO5300Display.h>

std::shared_ptr<tt::hal::touch::TouchDevice> createTouch() {
    auto configuration = std::make_unique<Ft6x36Touch::Configuration>(
        I2C_NUM_0,
        GPIO_NUM_38,
        LCD_HORIZONTAL_RESOLUTION,
        LCD_VERTICAL_RESOLUTION
    );

    auto touch = std::make_shared<Ft6x36Touch>(std::move(configuration));
    return std::reinterpret_pointer_cast<tt::hal::touch::TouchDevice>(touch);
}

std::shared_ptr<tt::hal::display::DisplayDevice> createDisplay() {
    // TODO: Correctly Replace Init Code with what driver has
//    auto configuration = std::make_unique<CO5300Display::Configuration>(
//         GPIO_NUM_4,
//         GPIO_NUM_12,
//         GPIO_NUM_NC,
//         GPIO_NUM_8,
//         80,
//         160,
//         nullptr,
//         false,
//         false,
//         false,
//         true,
//         0,
//         26,
//         1
//     );

    configuration->backlightDutyFunction = driver::pwmbacklight::setBacklightDuty;

    auto display = std::make_shared<CO5300Display>(std::move(configuration));
    return std::reinterpret_pointer_cast<tt::hal::display::DisplayDevice>(display);
}