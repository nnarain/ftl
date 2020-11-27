//
// i2c.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 02 2020
//

#ifndef FTL_PLATFORM_AVR_SUPPORT_I2C_HPP
#define FTL_PLATFORM_AVR_SUPPORT_I2C_HPP

#include <ftl/comms/i2c.hpp>

#include <stdint.h>

namespace ftl
{
namespace platform
{
namespace avr
{
namespace i2c
{
    using ClockMode = comms::i2c::ClockMode;
    using SlaMode = comms::i2c::SlaMode;

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
     * Get the I2C state
    */
    comms::i2c::State status();

    /**
     * I2C status to string
    */
    const char* statusToStr(uint8_t status);
}
}
}
} // namespace ftl

#endif
