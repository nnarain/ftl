//
// ATmega328p Setup
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 21 2020
//

#include <avr/io.h>
#include <util/delay.h>

#include <stdint.h>
#include <stdio.h>

#include <ftl/logging/logger.hpp>
#include <ftl/comms/uart.hpp>

#include <ftl/platform/avr/atmega328p/hardware.hpp>


using namespace ftl::logging;
using namespace ftl::platform::avr::atmega328p;

int main()
{
    Logger<Hardware::UART0> logger{ftl::comms::uart::BaudRate::Rate_9600};
    SystemLogger::instance().setLogger(&logger);

    int count = 0;

    for(;;)
    {
        LOG_INFO("count: %d", count);
        _delay_ms(1000);

        count++;
    }

    return 0;
}
