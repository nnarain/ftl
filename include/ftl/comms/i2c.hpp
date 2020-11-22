//
// comms/i2c.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 02 2020
//

#ifndef FTL_COMMS_I2C_HPP
#define FTL_COMMS_I2C_HPP

namespace ftl
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

#endif // FTL_COMMS_I2C_HPP
