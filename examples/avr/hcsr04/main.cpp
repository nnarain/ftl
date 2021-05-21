//
// HCSR04
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Apr 24 2021
//

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include <stdlib.h>

#include <ftl/logging/logger.hpp>
#include <ftl/comms/uart.hpp>
#include <ftl/drivers/sensors/hcsr04.hpp>
#include <ftl/platform/platform.hpp>

using namespace ftl::drivers;
using namespace ftl::logging;
using namespace ftl::platform;

int main(void)
{
    Logger<Hardware::UART0> logger{ftl::comms::uart::BaudRate::Rate_9600};
    SystemLogger::instance().setLogger(&logger);

    LOG_INFO("\n\n");

    Hcsr04<Hardware::GPIOD<7>, Hardware::InputCapture1, Hardware::Timer> hcsr04;

    // Enable global interrupts
    sei();

    while(1)
    {
        // Give 10us trigger pulse on trigger pin to HC-SR04
        const auto distance = hcsr04.read();

        LOG_INFO("distance: %0.04f m", distance);

        _delay_ms(1000);
    }
}