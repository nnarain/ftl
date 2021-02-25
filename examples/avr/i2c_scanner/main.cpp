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
#include <ftl/platform/avr/atmega2560/hardware.hpp>

using namespace ftl::logging;
using namespace ftl::platform::avr::atmega2560;

int main()
{
    Logger<Hardware::UART0> logger{ftl::comms::uart::BaudRate::Rate_9600};
    SystemLogger::instance().setLogger(&logger);

    // Arduino Mega - Pullup SDA, SCL
    Hardware::GpioD::OutputPin<0> scl_pullup;
    Hardware::GpioD::OutputPin<1> sda_pullup;

    sda_pullup.set();
    scl_pullup.set();

    LOG_INFO("Initializing I2C");

    Hardware::I2C::initialize();

    uint8_t address = 0x00;

    for(;;)
    {
        ftl::comms::i2c::I2CDevice<Hardware::I2C> device{address};

        if (device.detect())
        {
            LOG_INFO("Detected: %02X", address);
        }

        address += 2;

        Hardware::Timer::delayMs(10);
    }

    return 0;
}
