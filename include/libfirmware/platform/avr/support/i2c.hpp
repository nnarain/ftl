//
// i2c.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 02 2020
//

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

    // TODO: I2C device detection (start, send addr, recv ACK, end)
    // TODO: Handle bus states after each action

    /**
     * Initialize I2C
    */
    void init(ClockMode clock = ClockMode::Normal);
    /**
     * Send a START condition
    */
    uint8_t start(uint8_t address, SlaMode rw);
    /**
     * Send a STOP condition
    */
    void stop();
    /**
     * Send the slave address and read/write mode on the bus
    */
    // void slarw(uint8_t addr, uint8_t rw);
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
