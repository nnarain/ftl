//
// i2c_device.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 09 2020
//
#ifndef FTL_COMMS_I2C_I2C_DEVICE_HPP
#define FTL_COMMS_I2C_I2C_DEVICE_HPP

#include <stdint.h>

#include <ftl/comms/i2c.hpp>

namespace ftl
{
namespace comms
{
namespace i2c
{
    /**
     * Generic I2C device that can be written to or read from
    */
    template<class I2C>
    class I2CDevice
    {
    public:
        I2CDevice(uint8_t address)
            : address_{address}
        {
        }

        /**
         * Attempt to gain control of bus, and if that succeeds send SLA+RW.
         * 
         * Returns true on success, false otherwise.
        */
        bool begin(SlaMode mode)
        {
            // Send a start condition
            i2c_.start();

            State state = i2c_.status();

            // Confirm we are in the START or REPEATED-START states
            if (state != State::Start && state != State::RepeatedStart)
            {
                return false;
            }

            // Send the device address + SLA mode on the I2C bus
            i2c_.write((address_ << 1) | static_cast<uint8_t>(mode));

            state = i2c_.status();

            // Confirm that the I2C device responded with an ACK for the address
            if (state != State::MT_SlaveAck)
            {
                return false;
            }

            return true;
        }

        void end()
        {
            i2c_.stop();
        }

        void write(const uint8_t data)
        {
            write(&data, 1);
        }

        void write(const uint8_t* data, unsigned int len)
        {
            begin(SlaMode::Write);
            while(len--)
            {
                i2c_.write(*data++);
            }
            end();
        }

        void write1(uint8_t first, const uint8_t* data, unsigned int len)
        {
            begin(SlaMode::Write);
            i2c_.write(first);
            while(len--)
            {
                i2c_.write(*data++);
            }
            end();
        }

        void read(uint8_t* const data, unsigned long len)
        {
            begin(SlaMode::Read);
            for (auto i = 0u; i < len; ++i)
            {
                data[i] = i2c_.read(i < len - 1);
            }
            end();
        }

        bool detect()
        {
            if (begin(SlaMode::Write))
            {
                end();
                return true;
            }
            return false;
        }

    private:
        I2C i2c_;
        uint8_t address_;
    };
}
}
}

#endif
