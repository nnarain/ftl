//
// Adafruit Motor Shield V2
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date May 01 2021
//

#include <stdint.h>
#include <stdio.h>

#include <ftl/logging/logger.hpp>
#include <ftl/comms/uart.hpp>

// #include <ftl/drivers/pwm/pca9685.hpp>
// #include <ftl/drivers/motor_control/tb6612fng.hpp>
#include <ftl/drivers/adafruit/motor_shield.hpp>

#include <ftl/platform/platform.hpp>

using namespace ftl::drivers;
using namespace ftl::logging;
using namespace ftl::platform;

int main()
{
    Logger<Hardware::UART0> logger{ftl::comms::uart::BaudRate::Rate_9600};
    SystemLogger::instance().setLogger(&logger);

    LOG_INFO("\n\n");

    Hardware::I2C0::initialize(ftl::comms::i2c::ClockMode::Fast);

    MotorShield<Hardware::I2C0> motors{0x70};
    motors.getDriver().enable(false);
    Hardware::Timer::delayMs(1);

    if (!motors.initialize(1000))
    {
        LOG_ERROR("failed to init");
    }

    LOG_INFO("Starting motor example");

    for(;;)
    {
        motors.forward(0);
        Hardware::Timer::delayMs(2500);
        motors.backward(0);
        Hardware::Timer::delayMs(2500);
    }

    return 0;
}
