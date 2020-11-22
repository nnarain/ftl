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

        void write(const uint8_t data)
        {
            write(&data, 1);
        }

        void write(const uint8_t* data, unsigned int len)
        {
            i2c_.begin(address_, SlaMode::Write);
            while(len--)
            {
                i2c_.write(*data++);
            }
            i2c_.end();
        }

        void write1(uint8_t first, const uint8_t* data, unsigned int len)
        {
            i2c_.begin(address_, SlaMode::Write);
            i2c_.write(first);
            while(len--)
            {
                i2c_.write(*data++);
            }
            i2c_.end();
        }

        void read(uint8_t* const data, unsigned long len)
        {
            i2c_.begin(address_, SlaMode::Read);
            for (auto i = 0u; i < len; ++i)
            {
                data[i] = i2c_.read(i < len - 1);
            }
            i2c_.end();
        }

    private:
        I2C i2c_;
        uint8_t address_;
    };
}
}
}

#endif
