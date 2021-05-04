//
// motor_shield.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date May 02 2021
//

#ifndef FTL_DRIVERS_ADAFRUIT_MOTOR_SHIELD_HPP
#define FTL_DRIVERS_ADAFRUIT_MOTOR_SHIELD_HPP

#include <ftl/drivers/pwm/pca9685.hpp>
#include <ftl/drivers/motor_control/tb6612fng.hpp>

namespace ftl
{
namespace drivers
{

/**
 * Pin mapping for PCA9685 to TB6612FNG on the Adafruit Motor Shield
 */
struct MotorShieldDriver1PinPackage
{
    static constexpr uint8_t A_IN1 = 10;
    static constexpr uint8_t A_IN2 = 9;
    static constexpr uint8_t A_PWM = 8;
    static constexpr uint8_t B_IN1 = 11;
    static constexpr uint8_t B_IN2 = 12;
    static constexpr uint8_t B_PWM = 13;

    static constexpr uint8_t STBY  = 255;
};

struct MotorShieldDriver2PinPackage
{
    static constexpr uint8_t A_IN1 = 4;
    static constexpr uint8_t A_IN2 = 3;
    static constexpr uint8_t A_PWM = 2;
    static constexpr uint8_t B_IN1 = 5;
    static constexpr uint8_t B_IN2 = 6;
    static constexpr uint8_t B_PWM = 7;

    static constexpr uint8_t STBY  = 255;
};

enum class MotorShieldDriveCommands
{
    Forward, Backward, Brake, Stop
};

/**
 * Adafruit Motor Shield Driver
 */
template<typename I2C>
class MotorShield
{
    // The PCA9685 can act as a GPIO group and a PWM group
    using PinDriver = Pca9685<I2C>;
    using MotorDriver1 = Tb6612fng<PinDriver, PinDriver, MotorShieldDriver1PinPackage>;
    using MotorDriver2 = Tb6612fng<PinDriver, PinDriver, MotorShieldDriver2PinPackage>;

public:
    MotorShield(uint8_t address) : driver_{address}, motor1_{driver_, driver_}, motor2_{driver_, driver_}
    {
    }

    bool initialize(float freq)
    {
        if (!driver_.initialize())
        {
            return false;
        }

        driver_.setFrequency(freq);
        driver_.enable(true);

        return true;
    }

    void forward(uint8_t mtr)
    {
        command(mtr, MotorShieldDriveCommands::Forward);
    }

    void backward(uint8_t mtr)
    {
        command(mtr, MotorShieldDriveCommands::Backward);
    }

    void brake(uint8_t mtr)
    {
        command(mtr, MotorShieldDriveCommands::Brake);
    }

    void stop(uint8_t mtr)
    {
        command(mtr, MotorShieldDriveCommands::Stop);
    }

    void command(uint8_t mtr, MotorShieldDriveCommands cmd)
    {
        switch (mtr)
        {
        case 0:
        case 1:
            motor1_.command(mtr % 2, static_cast<Tb6612FNGDriveCommand>(cmd));
            break;
        case 2:
        case 3:
            motor2_.command(mtr % 2, static_cast<Tb6612FNGDriveCommand>(cmd));
        default:
            break;
        }
    }

    void setSpeed(uint8_t mtr, float speed)
    {
        switch (mtr)
        {
        case 0:
        case 1:
            motor1_.setSpeed(mtr % 2, speed);
            break;
        case 2:
        case 3:
            motor1_.setSpeed(mtr % 2, speed);
        default:
            break;
        }
    }

    PinDriver& getDriver()
    {
        return driver_;
    }

private:
    PinDriver driver_;
    MotorDriver1 motor1_;
    MotorDriver2 motor2_;
};

}
}

#endif // FTL_DRIVERS_ADAFRUIT_MOTOR_SHIELD_HPP
