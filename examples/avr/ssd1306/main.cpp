//
// SSD1306 driver for ATmega2560
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 21 2020
//

#include <stdint.h>
#include <stdio.h>

#include <ftl/logging/logger.hpp>
#include <ftl/comms/uart.hpp>
#include <ftl/comms/i2c/i2c_device.hpp>

#include <ftl/drivers/displays/ssd1306.hpp>
#include <ftl/gfx/adaptors/ssd1306_display.hpp>

#include <ftl/platform/platform.hpp>

// XBITMAP sprites
#include "dino.h"

#define OLED_ADDRESS 0x3C
#define DISPLAY_HEIGHT 64

using namespace ftl::drivers;
using namespace ftl::logging;
using namespace ftl::platform;

int main()
{
    Logger<Hardware::UART0> logger{ftl::comms::uart::BaudRate::Rate_9600};
    SystemLogger::instance().setLogger(&logger);

    Hardware::I2C0::initialize(ftl::comms::i2c::ClockMode::Fast);

    ftl::gfx::Ssd1306Display<Hardware::I2C0> display{OLED_ADDRESS, DISPLAY_HEIGHT};
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
