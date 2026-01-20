#pragma once


#include <driver/gpio.h>
#include <lvgl.h>
#include <esp_lcd_co5300.h>
#include <Tactility/hal/display/DisplayDevice.h>
#include <Tactility/hal/spi/Spi.h>
#include <EspLcdSpiDisplay.h>
#include <functional>

class CO5300Display final  {
    
    std::shared_ptr<tt::Lock> lock;

public:
        class Configuration {
        public:

        Configuration(
        unsigned int horizontalResolution,
        unsigned int verticalResolution,
        int gapX = 0,
        int gapY = 0,
        bool swapXY = false,
        bool mirrorX = false,
        bool mirrorY = false,
        bool invertColor = false,
        std::shared_ptr<tt::hal::touch::TouchDevice> touch = nullptr,
        std::function<void(uint8_t)> _Nullable backlightDutyFunction = nullptr,
        bool lvglSwapBytes = false,
        lcd_rgb_element_order_t rgbElementOrder = LCD_RGB_ELEMENT_ORDER_RGB,
        spi_host_device_t spiHostDevice = SPI2_HOST,
        gpio_num_t clkPin = GPIO_NUM_NC,
        gpio_num_t resetPin = GPIO_NUM_NC,
        gpio_num_t csPin = GPIO_NUM_NC,
        gpio_num_t dcPin = GPIO_NUM_NC,
        gpio_num_t d0Pin = GPIO_NUM_NC,
        gpio_num_t d1Pin = GPIO_NUM_NC,
        gpio_num_t d2Pin = GPIO_NUM_NC,
        gpio_num_t d3Pin = GPIO_NUM_NC,
        unsigned int bits_per_pixel = 16,
        unsigned int use_qspi_interface = 0
        ) : 
        horizontalResolution(horizontalResolution),
        verticalResolution(verticalResolution),
        gapX(gapX),
        gapY(gapY),
        swapXY(swapXY),
        mirrorX(mirrorX),
        mirrorY(mirrorY),
        invertColor(invertColor),
        touch(touch),
        backlightDutyFunction(backlightDutyFunction),
        lvglSwapBytes(lvglSwapBytes),
        rgbElementOrder(rgbElementOrder),
        spiHostDevice(spiHostDevice),
        clkPin(clkPin),
        resetPin(resetPin),
        csPin(csPin),
        dcPin(dcPin),
        d0Pin(d0Pin),
        d1Pin(d1Pin),
        d2Pin(d2Pin),
        d3Pin(d3Pin),
        bits_per_pixel(bits_per_pixel),
        use_qspi_interface(use_qspi_interface)
        {

        }

        unsigned int horizontalResolution;
        unsigned int verticalResolution;
        int gapX;
        int gapY;
        bool swapXY;
        bool mirrorX;
        bool mirrorY;
        bool invertColor;
        std::shared_ptr<tt::hal::touch::TouchDevice> touch;
        std::function<void(uint8_t)> _Nullable backlightDutyFunction;
        bool lvglSwapBytes;
        lcd_rgb_element_order_t rgbElementOrder;
        spi_host_device_t spiHostDevice;
        gpio_num_t clkPin;
        gpio_num_t resetPin;
        gpio_num_t csPin;
        gpio_num_t dcPin;
        gpio_num_t d0Pin;
        gpio_num_t d1Pin;
        gpio_num_t d2Pin;
        gpio_num_t d3Pin;
        unsigned int bits_per_pixel;
        unsigned int use_qspi_interface;
    };
   
private:
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_handle_t panel_handle = NULL;
    std::unique_ptr<Configuration> configuration;
    esp_lcd_panel_io_spi_config_t createPanelConfig();
    bool createPanelHandle(esp_lcd_panel_io_handle_t ioHandle, esp_lcd_panel_handle_t& panelHandle);

public:
    explicit CO5300Display(std::unique_ptr<Configuration> inConfiguration) : configuration(std::move(inConfiguration)) {
        assert(configuration != nullptr);
        bool result = createPanelHandle(io_handle,panel_handle);
        if(result){
            ESP_LOGI("VGA","DONE 1");
        esp_lcd_panel_reset(panel_handle);
                    ESP_LOGI("VGA","DONE 2");

        esp_lcd_panel_init(panel_handle);
                    ESP_LOGI("VGA","DONE 3");
        esp_lcd_panel_disp_on_off(panel_handle, true);
                    ESP_LOGI("VGA","DONE 4");

        }
    }

    std::string getName() const  {
        return "CO5300";
    }

    std::string getDescription() const  {
        return "CO5300 AMOLED panel";
    }
};

std::shared_ptr<tt::hal::display::DisplayDevice> createDisplay();
