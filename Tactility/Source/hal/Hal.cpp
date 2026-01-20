#include <Tactility/Tactility.h>
#include <Tactility/hal/Configuration.h>
#include <Tactility/hal/Device.h>
#include <Tactility/hal/gps/GpsInit.h>
#include <Tactility/hal/i2c/I2cInit.h>
#include <Tactility/hal/power/PowerDevice.h>
#include <Tactility/hal/spi/SpiInit.h>
#include <Tactility/hal/uart/UartInit.h>
#include <Tactility/Logger.h>
#include <Tactility/hal/display/DisplayDevice.h>
#include <Tactility/hal/sdcard/SdCardMounting.h>
#include <Tactility/hal/touch/TouchDevice.h>
#include <Tactility/kernel/SystemEvents.h>

namespace tt::hal {

static const auto LOGGER = Logger("Hal");

void registerDevices(const Configuration& configuration) {
    LOGGER.info("Registering devices");

    auto devices = configuration.createDevices();
    for (auto& device : devices) {
        LOGGER.info("Registering device");
        if (device->getType() == Device::Type::I2c) LOGGER.info("I2C");
        if (device->getType() == Device::Type::Touch) ESP_LOGI("REGISTER","Touch");
        if (device->getType() == Device::Type::Display) ESP_LOGI("REGISTER","Display");
        if (device->getType() == Device::Type::SdCard) ESP_LOGI("REGISTER","SdCard");
        if (device->getType() == Device::Type::Keyboard) ESP_LOGI("REGISTER","Keyboard");
        if (device->getType() == Device::Type::Encoder) ESP_LOGI("REGISTER","Encoder");
        if (device->getType() == Device::Type::Power) ESP_LOGI("REGISTER","Power");
        if (device->getType() == Device::Type::Gps) ESP_LOGI("REGISTER","GPS");
        if (device->getType() == Device::Type::Other) ESP_LOGI("REGISTER","Other");
        registerDevice(device);
        LOGGER.info("Finished Registering device");

        // Register attached devices
        if (device->getType() == Device::Type::Display) {
            const auto display = std::static_pointer_cast<display::DisplayDevice>(device);
            assert(display != nullptr);
            ESP_LOGI("DISP","DISP NOT NULL");
            const std::shared_ptr<Device> touch = display->getTouchDevice();
            if (touch != nullptr) {
                registerDevice(touch);
            }
            ESP_LOGI("Touch","DISP touch NOT NULL");
        }
    }
}

static void startDisplays() {
    LOGGER.info("Starting displays & touch");
    auto displays = hal::findDevices<display::DisplayDevice>(Device::Type::Display);
    for (auto& display : displays) {
        LOGGER.info("{} starting", display->getName());
        if (!display->start()) {
            LOGGER.error("{} start failed", display->getName());
        } else {
            LOGGER.info("{} started", display->getName());

            if (display->supportsBacklightDuty()) {
                LOGGER.info("Setting backlight");
                display->setBacklightDuty(0);
            }

            auto touch = display->getTouchDevice();
            if (touch != nullptr) {
                LOGGER.info("{} starting", touch->getName());
                if (!touch->start()) {
                    LOGGER.error("{} start failed", touch->getName());
                } else {
                    LOGGER.info("{} started", touch->getName());
                }
            }
        }
    }
}

void init(const Configuration& configuration) {
    kernel::publishSystemEvent(kernel::SystemEvent::BootInitHalBegin);

    kernel::publishSystemEvent(kernel::SystemEvent::BootInitI2cBegin);
    tt_check(i2c::init(configuration.i2c), "I2C init failed");
    kernel::publishSystemEvent(kernel::SystemEvent::BootInitI2cEnd);

    kernel::publishSystemEvent(kernel::SystemEvent::BootInitSpiBegin);
    tt_check(spi::init(configuration.spi), "SPI init failed");
    kernel::publishSystemEvent(kernel::SystemEvent::BootInitSpiEnd);

    kernel::publishSystemEvent(kernel::SystemEvent::BootInitUartBegin);
    tt_check(uart::init(configuration.uart), "UART init failed");
    kernel::publishSystemEvent(kernel::SystemEvent::BootInitUartEnd);

    if (configuration.initBoot != nullptr) {
        LOGGER.info("Init power");
        tt_check(configuration.initBoot(), "Init power failed");
    }

    registerDevices(configuration);

    sdcard::mountAll(); // Warning: This needs to happen BEFORE displays are initialized on the SPI bus

    startDisplays(); // Warning: SPI displays need to start after SPI SD cards are mounted

    kernel::publishSystemEvent(kernel::SystemEvent::BootInitHalEnd);
}

const Configuration* getConfiguration() {
    return tt::getConfiguration()->hardware;
}

} // namespace
