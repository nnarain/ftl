//
// pca9685_servo.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Apr 26 2021
//

#include <ftl/drivers/pwm/pca9685.hpp>

#include <stdint.h>

namespace ftl
{
namespace motor_control
{
/**
 * Adapts a PCA9685 channel into a PWM interface acceptable to a generic servo controller
 */
template<typename I2C>
class Pca9685ServoAdaptor
{
public:
    template<typename... Args>
    Pca9685ServoAdaptor(uint8_t addr, uint8_t channel) : driver_{addr}, channel_{channel}
    {
    }

    bool initialize()
    {
        if (!driver_.initialize())
        {
            return false;
        }

        return true;
    }

    void setFrequency(float freq)
    {
        driver_.setFrequency(freq);
    }

    void setDutyCycle(float duty)
    {
        driver_.setDutyCycle(channel_, duty);
    }

    drivers::Pca9685<I2C>& getDriver()
    {
        return driver_;
    }

private:
    drivers::Pca9685<I2C> driver_;
    uint8_t channel_;
};
}
}
