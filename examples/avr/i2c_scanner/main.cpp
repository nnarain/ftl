//
// I2C scanner for ATmega2560
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 22 2020
//

#include <stdint.h>
#include <stdio.h>

#include <ftl/logging/logger.hpp>
#include <ftl/comms/uart.hpp>
#include <ftl/comms/i2c/i2c_device.hpp>
#include <ftl/platform/platform.hpp>

using namespace ftl::logging;
using namespace ftl::platform;

int main()
{
    Logger<Hardware::UART0> logger{ftl::comms::uart::BaudRate::Rate_9600};
    SystemLogger::instance().setLogger(&logger);

    LOG_INFO("Initializing I2C");

    Hardware::I2C0::initialize();

    LOG_INFO("Starting scan...");

    for(;;)
    {
        auto devices = 0;

        for (auto address = 1; address < 127; address++)
        {
            ftl::comms::i2c::I2CDevice<Hardware::I2C0> device{address};

            if (device.detect())
            {
                LOG_INFO("Detected: %02X", address);
                devices++;
            }
        }

        LOG_INFO("%d devices found!", devices);

        Hardware::Timer::delayMs(1000);
    }

    return 0;
}
