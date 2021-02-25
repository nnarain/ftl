//
// ATmega328p Setup
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 21 2020
//

#include <stdint.h>
#include <stdio.h>

#include <ftl/comms/uart.hpp>
#include <ftl/logging/logger.hpp>

#include <ftl/platform/platform.hpp>

using namespace ftl::logging;
using namespace ftl::platform;

int main()
{
    Logger<Hardware::UART0> logger{ftl::comms::uart::BaudRate::Rate_9600};
    SystemLogger::instance().setLogger(&logger);

    int count = 0;

    for(;;)
    {
        LOG_INFO("count: %d", count);
        Hardware::Timer::delayMs(1000);

        count++;
    }

    return 0;
}
