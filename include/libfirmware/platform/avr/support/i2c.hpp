//
// i2c.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 02 2020
//

#ifndef LIBFIRMWARE_PLATFORM_AVR_SUPPORT_I2C_HPP
#define LIBFIRMWARE_PLATFORM_AVR_SUPPORT_I2C_HPP

#include <libfirmware/comms/i2c.hpp>

#include <stdint.h>

namespace libfirmware
{
namespace platform
{
namespace avr
{
namespace i2c
{
    using ClockMode = comms::i2c::ClockMode;
    using SlaMode = comms::i2c::SlaMode;

    enum class Status
    {
        OK = 0,

    };

    /**
     * Initialize I2C
    */
    void init(ClockMode clock = ClockMode::Normal);
    /**
     * Start I2C transaction
    */
    uint8_t begin(uint8_t address, SlaMode rw);
    /**
     * End I2C transaction
    */
    void end();
    /**
     * Send START condition
    */
    void start();
    /**
     * Send a STOP condition
    */
    void stop();
    /**
     * Write a byte to the bus
    */
    void write(uint8_t data);
    /**
     * Write a buffer to the bus
    */
    void write(const uint8_t* data, uint32_t len);
    /**
     * Read a byte from the bus
    */
    uint8_t read(bool ack = true);
    /**
    * Read buffer from the bus
    */
    void read(uint8_t* data, uint32_t len);

    /**
     * I2C status to string
    */
    const char* statusToStr(uint8_t status);
}
}
}
} // namespace libfirmware

#endif
