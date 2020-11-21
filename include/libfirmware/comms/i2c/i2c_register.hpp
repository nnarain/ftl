//
// i2c_device.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 09 2020
//
#ifndef LIBFIRMWARE_COMMS_I2C_I2C_REGISTER_HPP
#define LIBFIRMWARE_COMMS_I2C_I2C_REGISTER_HPP

#include <stdint.h>

#include "i2c_device.hpp"

namespace libfirmware
{
namespace comms
{
namespace i2c
{
enum class Endian
{
    Big, Little,
};

/**
 * I2C Register Pattern
 * 
 * A common pattern used in I2C communication is having the master select a register (by writing a value), and then reading
 * the register value back from the slave
*/
template<class I2C, unsigned int L, Endian E = Endian::Big>
class Register
{
public:
    using Device = I2CDevice<I2C>;
    static constexpr unsigned int LENGTH = L;

    Register(Device& dev, uint8_t reg)
        : dev_{dev}
        , reg_{reg}
    {
    }

    /**
     * Read from the register
    */
    void read(uint8_t* const data)
    {
        // Write the register to read from
        dev_.write(reg_);
        // Read the data from the register
        dev_.read(data, LENGTH);
    }

    template<typename T>
    T readType()
    {
        static_assert(sizeof(T) >= LENGTH, "Return type must be greater than or equal to the register size");

        uint8_t recv[LENGTH];
        read(recv);

        // Convert byte order
        T result = 0;

        for (auto i = 0u; i < LENGTH; ++i)
        {
            result <<= 8;
            switch (E)
            {
            case Endian::Big:
                result |= recv[i];
                break;
            case Endian::Little:
                result |= recv[LENGTH - i - 1];
                break;
            }
        }

        return result;
    }

    /**
     * Write to the register
    */
    void write(uint8_t* const data)
    {
        // dev_.write(reg_);
        // dev_.write(data, LENGTH);
        dev_.write1(reg_, data, LENGTH);
    }

    template<typename T>
    void writeType(T data)
    {
        static_assert(sizeof(T) >= LENGTH, "Return type must be greater than or equal to the register size");

        uint8_t send[LENGTH] = {0};
        for (auto i = 0u; i < LENGTH; ++i)
        {
            switch (E)
            {
            case Endian::Big:
                send[LENGTH - i - 1] = data & 0xFF;
                break;
            case Endian::Little:
                send[i] = data & 0xFF;
                break;
            }

            data >>= 8;
        }

        write(send);
    }

    /**
     * Modify a portion of bits in the register
     * \param new_value New value to 
    */
    template<typename T>
    void writeBits(T new_value, T mask, uint8_t shift)
    {
        // Read the current register value
        const auto current_value = readType<T>();
        // Modify the current value by shifting the new value in place
        const auto modified = (current_value & ~(mask << shift)) | ((new_value & mask) << shift);
        // Write the modified value back to the register
        writeType<T>(modified);
    }

    template<typename T>
    T readBits(T mask, uint8_t shift)
    {
        return (readType<T>() >> shift) & mask;
    }

private:
    Device& dev_;
    uint8_t reg_;
};

template<class I2C, typename DataT, Endian E = Endian::Big>
class TypedRegister
{
public:
    TypedRegister(I2CDevice<I2C>& dev, uint8_t reg)
        : reg_{dev, reg}
    {
    }

    DataT read()
    {
        return reg_.template readType<DataT>();
    }

    void write(const DataT data)
    {
        reg_.template writeType<DataT>(data);
    }

    void writeBits(const DataT data, const DataT mask, uint8_t shift)
    {
        reg_.template writeBits<DataT>(data, mask, shift);
    }

    DataT readBits(DataT mask, uint8_t shift)
    {
        return reg_.template readBits<DataT>(mask, shift);
    }

private:
    Register<I2C, sizeof(DataT), E> reg_;
};

template<class I2C>                         using Uint8Register  = TypedRegister<I2C, uint8_t, Endian::Big>;
template<class I2C>                         using Int8Register   = TypedRegister<I2C, int8_t, Endian::Big>;
template<class I2C, Endian E = Endian::Big> using Uint16Register = TypedRegister<I2C, uint16_t, E>;
template<class I2C, Endian E = Endian::Big> using Int16Register  = TypedRegister<I2C, int16_t,  E>;
template<class I2C, Endian E = Endian::Big> using Uint32Register = TypedRegister<I2C, uint32_t, E>;
template<class I2C, Endian E = Endian::Big> using Int32Register  = TypedRegister<I2C, int32_t,  E>;
template<class I2C, Endian E = Endian::Big> using Uint64Register = TypedRegister<I2C, uint64_t, E>;
template<class I2C, Endian E = Endian::Big> using Int64Register  = TypedRegister<I2C, int64_t,  E>;


}
}
}

#endif
