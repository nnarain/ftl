//
// SSD1306 driver for ATmega2560
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 21 2020
//

#include <stdint.h>
#include <stdio.h>

#include <ftl/logging/logger.hpp>
#include <ftl/comms/i2c/i2c_device.hpp>

#include <ftl/drivers/displays/ssd1306.hpp>
#include <ftl/gfx/adaptors/ssd1306_display.hpp>
#include <ftl/logging/adaptors/display_adaptor.hpp>
#include <ftl/gfx/fonts/basic_font.hpp>
#include <ftl/platform/avr/atmega32u4/hardware.hpp>

#define OLED_ADDRESS 0x3C
#define DISPLAY_HEIGHT 64

using namespace ftl::drivers;
using namespace ftl::logging;
using namespace ftl::platform::avr::atmega32u4;

int main()
{
    Hardware::I2C0::initialize(ftl::comms::i2c::ClockMode::Fast);

    Logger<RasterDisplayLoggerAdaptor<ftl::gfx::Ssd1306Display<Hardware::I2C0>>> logger{OLED_ADDRESS, DISPLAY_HEIGHT};
    logger.getOutput().getDisplay().setFont(&ftl::gfx::fonts::BASIC_FONT);

    SystemLogger::instance().setLogger(&logger);

    int count = 0;

    for(;;)
    {
        LOG_INFO("%d", count);
        count++;

        Hardware::Timer::delayMs(1000);
    }

    return 0;
}

