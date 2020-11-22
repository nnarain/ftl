//
// SSD1306 driver for ATmega2560
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 21 2020
//

#include <avr/io.h>
#include <util/delay.h>

#include <stdint.h>
#include <stdio.h>

#include <ftl/utils/logger.hpp>
#include <ftl/comms/i2c/i2c_device.hpp>
#include <ftl/platform/avr/atmega2560/hardware.hpp>

#include <ftl/drivers/displays/ssd1306.hpp>

#define OLED_ADDRESS 0x3C

using namespace ftl::drivers;
using namespace ftl::utils;
using namespace ftl::platform::avr::atmega2560;

int main()
{
    Logger<Hardware::Uart0> logger{Baud::Rate_9600};
    SystemLogger::instance().setLogger(&logger);

    // Arduino Mega - Pullup SDA, SCL
    Hardware::GpioD::OutputPin<0> scl_pullup;
    Hardware::GpioD::OutputPin<1> sda_pullup;

    // displays::Ssd1306<Hardware::I2C> oled{OLED_ADDRESS};
    ftl::comms::i2c::I2CDevice<Hardware::I2C> oled{OLED_ADDRESS};

    for(;;)
    {
        if (oled.detect())
        {
            LOG_INFO("Detected the OLED display!");
        }

        _delay_ms(1000);
    }

    return 0;
}
