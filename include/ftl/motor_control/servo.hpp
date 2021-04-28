//
// servo.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Apr 26 2021
//

#include <ftl/math/range.hpp>

#include <ftl/logging/logger.hpp>

namespace ftl
{
namespace motor_control
{

/**
 * Generic servo controller built on a PWM interface
 * 
 * \tparam PWM PWM interface
 */
template<typename PWM>
class Servo
{
public:
    template<typename... Args>
    Servo(Args... args) : pwm_{args...}
    {
    }

    bool initialize()
    {
        if (!pwm_.initialize())
        {
            return false;
        }

        pwm_.setFrequency(50.f);

        return true;
    }

    void setAngle(float angle)
    {
        if (angle < 0)
        {
            angle = 0;
        }
        else if (angle > 180)
        {
            angle = 180;
        }

        const auto duty = ftl::math::scale(angle, 0.0f, 180.0f, 0.05f, 0.1f);

        LOG_INFO("duty: %0.4f", duty);

        pwm_.setDutyCycle(duty);
    }

    PWM& getPwm()
    {
        return pwm_;
    }

private:
    PWM pwm_;
};

}
}
