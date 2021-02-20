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

#include <ftl/comms/uart.hpp>
#include <ftl/logging/logger.hpp>
#include <ftl/platform/avr/atmega32u4/hardware.hpp>

using namespace ftl::logging;
using namespace ftl::platform::avr::atmega32u4;

int main()
{
    Logger<Hardware::UART1> logger{ftl::comms::uart::BaudRate::Rate_9600};
    SystemLogger::instance().setLogger(&logger);

    for(;;)
    {
        LOG_INFO("Hello ATmega32u4!");
        _delay_ms(1000);
    }

    return 0;
}
