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

    Hardware::I2C::initialize();

    displays::Ssd1306<Hardware::I2C> oled{OLED_ADDRESS};

    if (oled.initialize())
    {
        LOG_INFO("OLED initialized!");
        oled.setAddresingMode(displays::Ssd1306_AddressingMode::Page);
    }

    uint8_t buf[128] = {0};
    memset(buf, 0xFF, sizeof(buf));

    for (auto i = 0; i < 8; i += 2)
    {
        oled.setPageStart(i);
        oled.sendBuffer(buf, sizeof(buf));
    }

    for(;;)
    {

        _delay_ms(1000);
    }

    return 0;
}
