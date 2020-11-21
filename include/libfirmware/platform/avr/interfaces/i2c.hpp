//
// platform/avr/atmega2560/i2c.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Oct 31 2020
//
#ifndef LIBFIRMWARE_PLATFORM_AVR_I2C_HPP
#define LIBFIRMWARE_PLATFORM_AVR_I2C_HPP

#include <libfirmware/comms/i2c.hpp>
#include <libfirmware/platform/avr/support/i2c.hpp>

namespace libfirmware
{
namespace platform
{
namespace avr
{
    class HardwareI2C
    {
    public:
        HardwareI2C()
        {
        }

        /**
         * 
        */
        static void initialize(comms::i2c::ClockMode clock = comms::i2c::ClockMode::Normal)
        {
            i2c::init(clock);
        }

        /**
         *
        */
        void begin(uint8_t address, comms::i2c::SlaMode mode)
        {
            i2c::begin(address, mode);
        }

        void end()
        {
            stop();
        }

        /**
         * Send I2C START condition
        */
        void start()
        {
            i2c::start();
        }

        /**
         * Send I2C STOP condition
        */
        void stop()
        {
            i2c::end();
        }

        /**
         * Send a byte on the I2C bus
        */
        void write(uint8_t data)
        {
            i2c::write(data);
        }

        /**
         * Read a byte from the I2C bus
        */
        uint8_t read(bool ack)
        {
            return i2c::read(ack);
        }
    };
}
}
} // namespace libfirmware

#endif // LIBFIRMWARE_PLATFORM_AVR_I2C_HPP
