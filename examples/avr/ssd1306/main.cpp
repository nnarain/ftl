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
#include <ftl/gfx/adaptors/ssd1306_display.hpp>

// XBITMAP sprites
#include "dino.h"

#define OLED_ADDRESS 0x3C

using namespace ftl::drivers;
using namespace ftl::utils;
using namespace ftl::platform::avr::atmega2560;

int main()
{
    Logger<Hardware::Uart0> logger{Baud::Rate_9600};
    SystemLogger::instance().setLogger(&logger);

    Hardware::I2C::initialize(ftl::comms::i2c::ClockMode::Fast);

    ftl::gfx::Ssd1306Display<Hardware::I2C> display{OLED_ADDRESS};
    if (display.initialize())
    {
        LOG_INFO("OLED init complete");
    }
    else
    {
        LOG_ERROR("Failed to initialized OLED display!");
    }


    display.clear();
    display.drawXBitmap(dino_bits, 0, 20, dino_width, dino_height, ftl::gfx::Color::white());
    display.update();

    // Enable hardware scrolling
    auto& driver = display.getDriver();
    driver.setupHorizontalScroll(Ssd1306_ScrollDirection::Right, 0, 7, Ssd1306_FrameInterval::FRAME_2);
    driver.scroll(true);

    for(;;)
    {
    }

    return 0;
}
