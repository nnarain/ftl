//
// MCP9808 driver for ATmega2560
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 21 2020
//

#include <stdint.h>
#include <stdio.h>

#include <ftl/logging/logger.hpp>
#include <ftl/comms/uart.hpp>
#include <ftl/comms/i2c/i2c_device.hpp>
#include <ftl/platform/avr/atmega2560/hardware.hpp>

#include <ftl/drivers/sensors/mcp9808.hpp>

#define MCP9808_ADDRESS 0x18

using namespace ftl::drivers;
using namespace ftl::logging;
using namespace ftl::platform::avr::atmega2560;

int main()
{
    Logger<Hardware::UART0> logger{ftl::comms::uart::BaudRate::Rate_9600};
    SystemLogger::instance().setLogger(&logger);

    Hardware::I2C::initialize();

    sensors::Mcp9808<Hardware::I2C> mcp{MCP9808_ADDRESS};

    if (mcp.verify())
    {
        LOG_INFO("Sensor detected!");
        mcp.enable(true);
    }
    else
    {
        LOG_INFO("Sensor not detected!!! Check wiring");
        for(;;);
    }

    for(;;)
    {
        const auto temp = mcp.getAmbientTemperature();

        LOG_INFO("Temperature: %0.2f", temp);

        Hardware::Timer::delayMs(1000);
    }

    return 0;
}
