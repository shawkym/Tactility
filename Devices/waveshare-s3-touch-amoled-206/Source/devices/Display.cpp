#include "Display.h"
#include <Ft6x36Touch.h>
#include <CO5300Display.h>

std::shared_ptr<tt::hal::touch::TouchDevice> createTouch() {
    auto configuration = std::make_unique<Ft6x36Touch::Configuration>(
        I2C_NUM_0,             // I2C port
        GPIO_NUM_38,            // INT pin
        AMOLED_WIDTH,
        AMOLED_HEIGHT
    );

    auto touch = std::make_shared<Ft6x36Touch>(std::move(configuration));
            ESP_LOGI("Touch<","DID TOUCH");
    return std::reinterpret_pointer_cast<tt::hal::touch::TouchDevice>(touch);
}

/* Factory */
std::shared_ptr<tt::hal::display::DisplayDevice> createDisplay()
{
        auto configuration = std::make_unique<CO5300Display::Configuration>(
        (unsigned int) AMOLED_WIDTH,
        (unsigned int) AMOLED_HEIGHT,
        0,
        0,
        false,
        false,
        false,
        false,
        createTouch(),
        nullptr,
        false,
        LCD_RGB_ELEMENT_ORDER_RGB,   
        SPI2_HOST,          
        GPIO_NUM_11,
        GPIO_NUM_8,
        GPIO_NUM_12,
        GPIO_NUM_NC,
        GPIO_NUM_4,
        GPIO_NUM_5,
        GPIO_NUM_6,
        GPIO_NUM_7,
        16,                                  
        1
       );

    auto display = std::make_shared<CO5300Display>(std::move(configuration));
    return std::reinterpret_pointer_cast<tt::hal::display::DisplayDevice>(display);
}

