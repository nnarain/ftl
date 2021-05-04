//
// tb6612fng.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date May 01 2021
//

#ifndef FTL_DRIVERS_TB6612FNG_HPP
#define FTL_DRIVERS_TB6612FNG_HPP

namespace ftl
{
namespace drivers
{
enum class Tb6612FNGDriveCommand
{
    Forward, Backward, Brake, Stop
};

/**
 * Driver IC for Dual DC Motors
 * 
 * \tparam GpioGroup A GPIO group interface
 * \tparam PWMGroup A PWM group interface
 * \tparam PinPackage The pin corresponding to the driver IC
 */
template<typename GpioGroup, typename PWMGroup, typename PinPackage>
class Tb6612fng
{
public:
    Tb6612fng(GpioGroup& gpio, PWMGroup& pwm) : gpio_{gpio}, pwm_{pwm}
    {
    }

    void initialize(float freq)
    {
        pwm_.setFrequency(freq);
    }

    void command(uint8_t mtr, Tb6612FNGDriveCommand cmd)
    {
        uint8_t in1, in2;

        switch(mtr)
        {
            case 0:
                in1 = PinPackage::A_IN1;
                in2 = PinPackage::A_IN2;
                break;
            case 1:
                in1 = PinPackage::B_IN1;
                in2 = PinPackage::B_IN2;
                break;
        }

        switch (cmd)
        {
        case Tb6612FNGDriveCommand::Forward:
            gpio_.setPinState(in1, false);
            gpio_.setPinState(in2, true);
            break;
        case Tb6612FNGDriveCommand::Backward:
            gpio_.setPinState(in1, true);
            gpio_.setPinState(in2, false);
            break;
        case Tb6612FNGDriveCommand::Brake:
        case Tb6612FNGDriveCommand::Stop:
            gpio_.setPinState(in1, false);
            gpio_.setPinState(in2, false);
            break;
        default:
            break;
        }
    }

    void setSpeed(uint8_t mtr, float speed)
    {
        switch (mtr)
        {
        case 0:
            pwm_.setDutyCycle(PinPackage::A_PWM, speed);
            break;
        case 1:
            pwm_.setDutyCycle(PinPackage::B_PWM, speed);
            break;
        default:
            // Invalid motor
            break;
        }
    }

private:
    GpioGroup& gpio_;
    PWMGroup& pwm_;
};

}
}

#endif // FTL_DRIVERS_TB6612FNG_HPP
