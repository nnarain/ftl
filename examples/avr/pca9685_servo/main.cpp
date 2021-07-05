//
// PCA9685 servo
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Apr 26 2021
//

#include <stdint.h>
#include <stdio.h>

#include <ftl/logging/logger.hpp>
#include <ftl/comms/uart.hpp>
#include <ftl/comms/i2c/i2c_device.hpp>

#include <ftl/motor_control/servo.hpp>
#include <ftl/motor_control/adaptors/pca9685_servo.hpp>

#include <ftl/platform/platform.hpp>

using namespace ftl::motor_control;
using namespace ftl::drivers;
using namespace ftl::logging;
using namespace ftl::platform;

int main()
{
    Logger<Hardware::UART0> logger{ftl::comms::uart::BaudRate::Rate_9600};
    SystemLogger::instance().setLogger(&logger);

    LOG_INFO("\n\n");
    Hardware::Timer::delayMs(100);

    Hardware::I2C0::initialize(ftl::comms::i2c::ClockMode::Fast);

    Servo<Pca9685ServoAdaptor<Hardware::I2C0>> servo{0x70, 0};

    // Put device to sleep explicitly
    servo.getPwm().getDriver().enable(false);
    servo.getPwm().getDriver().setOutputDrive(true);
    Hardware::Timer::delayMs(1);

    if (servo.initialize())
    {
        LOG_INFO("PCA9685 servo initialized");
    }
    else
    {
        LOG_ERROR("Failed to initialize PCA9685 servo");
        for(;;);
    }

    servo.getPwm().getDriver().enable(true);

    const auto prescale = servo.getPwm().getDriver().getPrescale();
    LOG_INFO("prescale: %d", prescale);

    for(;;)
    {
        servo.setAngle(0);
        Hardware::Timer::delayMs(1000);
        servo.setAngle(90);
        Hardware::Timer::delayMs(1000);
        servo.setAngle(180);
        Hardware::Timer::delayMs(1000);
    }

    return 0;
}
