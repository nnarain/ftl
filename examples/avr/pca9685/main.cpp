//
// PCA9685
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Apr 24 2021
//

#include <stdint.h>
#include <stdio.h>

#include <ftl/logging/logger.hpp>
#include <ftl/comms/uart.hpp>
#include <ftl/comms/i2c/i2c_device.hpp>

#include <ftl/drivers/pwm/pca9685.hpp>

#include <ftl/platform/platform.hpp>

using namespace ftl::drivers;
using namespace ftl::logging;
using namespace ftl::platform;

int main()
{
    Logger<Hardware::UART0> logger{ftl::comms::uart::BaudRate::Rate_9600};
    SystemLogger::instance().setLogger(&logger);

    Hardware::I2C0::initialize(ftl::comms::i2c::ClockMode::Fast);

    Pca9685<Hardware::I2C0> pwm{0x70};

    if (pwm.initialize())
    {
        LOG_INFO("PWM controller initialized");
    }
    else
    {
        LOG_ERROR("PWM controller failed to initialize");
    }

    // Set to 50Hz
    LOG_INFO("Setting freq to 50Hz");
    pwm.setFrequency(50.0f);



    LOG_INFO("Enabling PWM controller");
    pwm.enable(true);

    for(;;)
    {
        const auto prescale = pwm.getPrescale();
        LOG_INFO("prescale: %d", prescale);

        for (auto i = 0; i < 11; ++i)
        {
            float duty = (float)i * 0.1f;
            pwm.setDutyCycle(0, duty);
            Hardware::Timer::delayMs(100);
        }
        Hardware::Timer::delayMs(1000);
    }

    return 0;
}
