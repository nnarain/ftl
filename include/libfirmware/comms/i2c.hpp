//
// comms/i2c.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 02 2020
//

#ifndef LIBFIRMWARE_COMMS_I2C_HPP
#define LIBFIRMWARE_COMMS_I2C_HPP

namespace libfirmware
{
namespace comms
{
namespace i2c
{
    enum class ClockMode: unsigned long
    {
        Normal = 100000UL,
        Fast = 400000UL,
    };

    enum class SlaMode
    {
        Write = 0,
        Read = 1,
    };

    enum class State
    {
        Ok,
        BusError,
    };
}
}
}

#endif // LIBFIRMWARE_COMMS_I2C_HPP
