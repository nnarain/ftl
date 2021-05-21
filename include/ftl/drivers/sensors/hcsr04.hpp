//
// hcsr04.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date May 07 2021
//

#ifndef FTL_DRIVERS_HCSR04_HPP
#define FTL_DRIVERS_HCSR04_HPP

#include <ftl/gpio/gpio.hpp>

namespace ftl
{
namespace drivers
{

template<typename TriggerGPIO, typename PulseIn, typename Timer>
class Hcsr04
{
public:
    Hcsr04() : trigger_{ftl::GpioState::Output}
    {
        speed_of_sound_ = calcSpeedOfSound(25.0f);
    }

    // Maybe just do this with integer math?
    float read()
    {
        // Send a 10us pulse to the trigger input
        trigger_.set();
        Timer::delayUs(10);
        trigger_.reset();

        const auto pulse_width_us = input_.pulseIn();

        // distance = speed * time
        // time is to obstable and back. So divide by 2
        return ((pulse_width_us / 1000000.0f) * speed_of_sound_) / 2;
    }

    void updateAmbientTemperature(float temp)
    {
        speed_of_sound_ = calcSpeedOfSound();
    }

private:
    float calcSpeedOfSound(float temp)
    {
        return 331.0f + (0.6f * temp);
    }

    TriggerGPIO trigger_;
    PulseIn input_;

    float speed_of_sound_;
};

}
}

#endif // FTL_DRIVERS_HCSR04_HPP
