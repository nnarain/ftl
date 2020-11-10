//
// mcp9600.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 08 2020
//
#ifndef LIBFIRMWARE_DRIVERS_SENSORS_MCP9600_HPP
#define LIBFIRMWARE_DRIVERS_SENSORS_MCP9600_HPP

#include <libfirmware/comms/i2c/i2c_device.hpp>

namespace libfirmware
{
namespace drivers
{
namespace sensors
{
/**
 * Thermocouple EMF to Temperature Converter
 * +/- 1C Maximum accuracy
 * 
 * 
*/
template<class I2C>
class Mcp9600
{
    static constexpr uint8_t HOT_JUNCTION  = 0x00;
    static constexpr uint8_t COLD_JUNCTION = 0x02;
    static constexpr uint8_t DEVICE_ID     = 0x20;
    static constexpr uint8_t STATUS        = 0x02;
    static constexpr uint8_t CONFIG1       = 0x05;
    static constexpr uint8_t CONFIG2       = 0x06;
public:
    Mcp9600(uint8_t address)
        : dev_{address}
    {
    }

    bool initialize()
    {
        dev_.write(DEVICE_ID);

        uint8_t data[2] = { 0 };
        dev_.read(data, 2);

        return data[0] == 0x40;
    }

    int32_t readAmbient()
    {
        return readTemperature(COLD_JUNCTION);
    }

    int32_t readThermocouple()
    {
        return readTemperature(HOT_JUNCTION);
    }

private:
    int32_t readTemperature(uint8_t reg)
    {
        dev_.write(reg);

        uint8_t data[2] = { 0 };
        dev_.read(data, 2);

        return convertTemperature(data[0], data[1]);
    }

    int32_t convertTemperature(uint8_t msb, uint8_t lsb)
    {
        int32_t temperature = 0;

        if ((msb & 0x80) == 0x80)
        {
            msb &= 0x7F;
            temperature = 1024 - (msb * 16 + lsb / 16);
        }
        else
        {
            temperature = (msb * 16 + lsb / 16);
        }

        return temperature;
    }

    // Underlying I2C device
    comms::i2c::I2CDevice<I2C> dev_;
};
}
}
}

#endif // LIBFIRMWARE_DRIVERS_SENSORS_MCP9600_HPP
