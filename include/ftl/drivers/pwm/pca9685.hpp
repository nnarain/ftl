//
// pca9685.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Apr 25 2021
//

#ifndef FTL_DRIVERS_PWM_PCA9685_HPP
#define FTL_DRIVERS_PWM_PCA9685_HPP

#include <ftl/comms/i2c/i2c_device.hpp>

namespace ftl
{
namespace drivers
{
/**
 * 16 channel PWM controller over I2C
 * 
 * * Each channel has 12-bit resolution (4096 steps)
 * * Operating frequency: 24 Hz - 1526 Hz
 * * Duty cycle: 0% - 100%
 * 
 * \tparam I2C Host device I2C (2-wire) interface
 */
template<typename I2C>
class Pca9685
{
public:
    Pca9685(uint8_t addr) : device_{addr}
    {

    }
    ~Pca9685() = default;

    bool initialize()
    {
        return device_.detect();
    }

private:
    comms::i2c::I2CDevice<I2C> device_;
};
}
}

#endif // FTL_DRIVERS_PWM_PCA9685_HPP
