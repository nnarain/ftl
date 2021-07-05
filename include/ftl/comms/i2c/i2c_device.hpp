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
            // // Send a start condition
            // i2c_.start();

            // State state = i2c_.status();

            // // Confirm we are in the START or REPEATED-START states
            // if (state != State::Start && state != State::RepeatedStart)
            // {
            //     return false;
            // }

            // // Send the device address + SLA mode on the I2C bus
            // i2c_.write((address_ << 1) | static_cast<uint8_t>(mode));

            // state = i2c_.status();

            // // Confirm that the I2C device responded with an ACK for the address
            // if (mode == SlaMode::Write && state != State::MT_SlaveAck)
            // {
            //     return false;
            // }
            // else if (mode == SlaMode::Read && state != State::MR_SlaveAck)
            // {
            //     return false;
            // }

            i2c_.begin(address_, mode);

            return true;
        }

        /**
         * Sends a STOP condition to the I2C bus
        */
        void end()
        {
            i2c_.stop();
        }

        /**
         * Write a single byte to the bus
        */
        void write(const uint8_t data)
        {
            write(&data, 1);
        }

        void write(const uint8_t* data, unsigned int len)
        {
            while(len--)
            {
                i2c_.write(*data++);
            }
        }

        /**
         * Read a buffer from the bus
        */
        void read(uint8_t* const data, unsigned long len)
        {
            for (auto i = 0u; i < len; ++i)
            {
                data[i] = i2c_.read(i < len - 1);
            }
        }

        /**
         * Send a byte to the target device, automatically starting and ending the transaction
        */
        void sendByte(const uint8_t data)
        {
            sendBuffer(&data, 1);
        }

        /**
         * Send a byte buffer to the target device, automatically starting and ending the transaction
        */
        void sendBuffer(const uint8_t* data, unsigned int len)
        {
            begin(SlaMode::Write);
            write(data, len);
            end();
        }

        /**
         * Read a byte buffer to the target device, automatically starting and ending the transaction
        */
        void receiveBuffer(uint8_t* const data, unsigned long len)
        {
            begin(SlaMode::Read);
            read(data, len);
            end();
        }

        /**
         * Detect the device on the bus by sending START and SLA+W and checking the response
        */
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
