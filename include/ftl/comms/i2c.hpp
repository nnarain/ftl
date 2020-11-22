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

    // Possible I2C bus states
    enum class State
    {
        // I2C Bus is ready
        Ready,
        // START condition sent
        Start,
        // REPEATED START condition sent (a second start without a stop)
        RepeatedStart,
        // Master transmitter mode, slave address ack
        MT_SlaveAck,
        // Master transmitter mode, slave address nack
        MT_SlaveNAck,
        // Master transmitter, data ack
        MT_DataAck,
        // Master transmitter, data nack
        MT_DataNAck,
        // Master transmitter, arbitration lost
        MT_ArbitrationLost,
        // TODO: Master Receiver modes
    };
}
}
}

#endif // FTL_COMMS_I2C_HPP
