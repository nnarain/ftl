//
// mcp9600.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 08 2020
//
#ifndef LIBFIRMWARE_DRIVERS_SENSORS_MCP9600_HPP
#define LIBFIRMWARE_DRIVERS_SENSORS_MCP9600_HPP

#include <libfirmware/comms/i2c/i2c_device.hpp>
#include <libfirmware/comms/i2c/i2c_register.hpp>

#include <libfirmware/utils/bitutil.hpp>

namespace libfirmware
{
namespace drivers
{
namespace sensors
{
/**
 * Thermocouple types to bits for the MCP9600
*/
enum class Mcp9600_ThermocoupleType
{
    K = 0,
    J = 1,
    T = 2,
    N = 3,
    S = 4,
    E = 5,
    B = 6,
    R = 7,
};

/**
 * Shutdown modes for MCP9600
*/
enum class Mcp9600_OperationalMode
{
    Normal = 0,
    Shutdown = 1,
    Burst = 2,
};

/**
 * Number of temperature samples config
*/
enum class Mcp9600_TemperatureSamples
{
    Samples_1   = 0,
    Samples_2   = 1,
    Samples_4   = 2,
    Samples_8   = 3,
    Samples_16  = 4,
    Samples_32  = 5,
    Samples_64  = 6,
    Samples_128 = 7
};

/**
 * ADC Resolution config
*/
enum class Mcp9600_AdcResolution
{
    Res_18bits = 0,
    Res_16bits = 1,
    Res_14bits = 2,
    Res_12bits = 3,
};

/**
 * Cold junction sensor resolution
*/
enum class Mcp9600_ColdJunctionResolution
{
    Res_0_0625 = 0,
    Res_0_25 = 1,
};

/**
 * Thermocouple EMF to Temperature Converter
 * +/- 1C Maximum accuracy
 * 
 * 
*/
template<class I2C>
class Mcp9600
{
    static constexpr uint8_t HOT_JUNCTION_REGISTER  = 0x00;
    static constexpr uint8_t DELTA_REGISTER         = 0x01;
    static constexpr uint8_t COLD_JUNCTION_REGISTER = 0x02;
    static constexpr uint8_t ADC_REGISTER           = 0x03;
    static constexpr uint8_t STATUS_REGISTER        = 0x04;
    static constexpr uint8_t SENSOR_CONFIG_REGISTER = 0x05;
    static constexpr uint8_t DEVICE_CONFIG_REGISTER = 0x06;
    static constexpr uint8_t DEVICE_ID_REGISTER     = 0x20;
    static constexpr uint8_t ALERT1_CONFIG          = 0x08;
    static constexpr uint8_t ALERT2_CONFIG          = 0x09;
    static constexpr uint8_t ALERT3_CONFIG          = 0x0A;
    static constexpr uint8_t ALERT4_CONFIG          = 0x0B;
    static constexpr uint8_t ALERT1_HYST            = 0x0C;
    static constexpr uint8_t ALERT2_HYST            = 0x0D;
    static constexpr uint8_t ALERT3_HYST            = 0x0E;
    static constexpr uint8_t ALERT4_HYST            = 0x0F;
    static constexpr uint8_t ALERT1_LIMIT           = 0x10;
    static constexpr uint8_t ALERT2_LIMIT           = 0x11;
    static constexpr uint8_t ALERT3_LIMIT           = 0x12;
    static constexpr uint8_t ALERT4_LIMIT           = 0x13;

public:

    Mcp9600(uint8_t address)
        : dev_{address}
        , dev_id_{dev_, DEVICE_ID_REGISTER}
        , hot_junction_{dev_, HOT_JUNCTION_REGISTER}
        , cold_junction_{dev_, COLD_JUNCTION_REGISTER}
        , delta_{dev_, DELTA_REGISTER}
        , raw_adc_{dev_, ADC_REGISTER}
        , status_{dev_, STATUS_REGISTER}
        , sensor_config_{dev_, SENSOR_CONFIG_REGISTER}
        , device_config_{dev_, DEVICE_CONFIG_REGISTER}
    {
    }

    /**
     * Confirm device version
    */
    bool verify()
    {
        return getRevision() == 0x40;
    }

    /**
     * Get the device revision number
    */
    uint8_t getRevision()
    {
        return dev_id_.read();
    }

    /**
     * Read the ambient temperature (cold junction)
    */
    float readAmbient()
    {
        return convertTemperature(cold_junction_.read());
    }

    /**
     * Read the temperature at the thermocouple (hot junction)
    */
    float readThermocouple()
    {
        return convertTemperature(hot_junction_.read());
    }

    /**
     * Read the cold junction compensation register (delta)
    */
    float readDelta()
    {
        return convertTemperature(delta_.read());
    }

    /**
     * Get the raw ADC value
    */
    int32_t readAdc()
    {
        auto value = raw_adc_.template readType<int32_t>();

        if (value & BV(16))
        {
            value |= 0xFF000000;
        }

        return value;
    }

    /**
     * Check if burst conversion is complete
    */
    bool burstComplete()
    {
        return IS_BIT_SET(status_.read(), 7);
    }

    /**
     * Check if the temperature coversion is complete
    */
    bool conversionComplete()
    {
        return IS_BIT_SET(status_.read(), 6);
    }

    /**
     * Check if the detected voltage is outside the ADC limit
    */
    bool overVoltageDetected()
    {
        return IS_BIT_SET(status_.read(), 4);
    }

    /**
     * Get alert status (Allowed: 0 - 3)
    */
    bool alert(uint8_t index)
    {
        if (index <= 3)
        {
            return IS_BIT_SET(status_.read(), index);
        }
        else
        {
            return false;
        }
        
    }

    /**
     * Set thermocouple type
    */
    void setThermocouple(Mcp9600_ThermocoupleType type)
    {
        sensor_config_.writeBits(static_cast<uint8_t>(type), 0x07, 4);
    }

    /**
     * Get the thermocouple type
    */
    Mcp9600_ThermocoupleType getThermocoupleType()
    {
        return static_cast<Mcp9600_ThermocoupleType>(sensor_config_.readBits(0x07, 4));
    }

    /**
     * Set the filter coefficient
     * 
     * See datasheet for details
    */
    void setFilterCoefficient(uint8_t co)
    {
        sensor_config_.writeBits(co, 0x07, 0);
    }

    /**
     * Get the filter coefficient
    */
    uint8_t getFilterCoefficient()
    {
        return sensor_config_.readBits(0x07, 0);
    }

    /**
     * Set the operational mode
    */
    void setOperationalMode(Mcp9600_OperationalMode mode)
    {
        device_config_.writeBits(static_cast<uint8_t>(mode), 0x03, 0);
    }
    /**
     * Get the operational mode
    */
    Mcp9600_OperationalMode getOperationalMode()
    {
        return static_cast<Mcp9600_OperationalMode>(device_config_.readBits(0x03, 0));
    }

    /**
     * Set number of temperature samples to use
    */
    void setTemperatureSamples(Mcp9600_TemperatureSamples samples)
    {
        device_config_.writeBits(static_cast<uint8_t>(samples), 0x07, 2);
    }

    /**
     * Get the temperature samples configuration
    */
    Mcp9600_TemperatureSamples getTemperatureSamples()
    {
        return static_cast<Mcp9600_TemperatureSamples>(device_config_.readBits(0x07, 2));
    }

    /**
     * Set ADC resolution
    */
    void setAdcResolution(Mcp9600_AdcResolution res)
    {
        device_config_.writeBits(static_cast<uint8_t>(res), 0x03, 5);
    }

    /**
     * Get the ADC resolution
    */
    Mcp9600_AdcResolution getAdcResolution()
    {
        return device_config_.readBits(0x03, 5);
    }

    /**
     * Set the cold junction sensor resolution
    */
    void setColdJunctionResolution(Mcp9600_ColdJunctionResolution res)
    {
        device_config_.writeBits(static_cast<uint8_t>(res), 0x01, 7);
    }

    /**
     * Get the cold junction sensor resolution
    */
    Mcp9600_ColdJunctionResolution getColdJunctionResolution()
    {
        return static_cast<Mcp9600_ColdJunctionResolution>(device_config_.readBits(0x01, 7));
    }

    /**
     * Set alert temperature
    */
    void setAlertTemperature(uint8_t alert, float temperature)
    {
        if (alert > 3)
            return;

        comms::i2c::Int16Register<I2C> reg{dev_, ALERT1_LIMIT + alert};
        reg.write(static_cast<int16_t>(temperature / 0.0625));
    }

private:
    float convertTemperature(int16_t value)
    {
        float t = static_cast<float>(value);
        t *= 0.0625;

        return t;
    }

    // Underlying I2C device
    comms::i2c::I2CDevice<I2C> dev_;

    /* Registers */

    // TODO: Create these on demand?
    comms::i2c::Uint8Register<I2C> dev_id_;
    comms::i2c::Int16Register<I2C> hot_junction_;
    comms::i2c::Int16Register<I2C> cold_junction_;
    comms::i2c::Int16Register<I2C> delta_;
    comms::i2c::Register<I2C, 3> raw_adc_;
    comms::i2c::Uint8Register<I2C> status_;
    comms::i2c::Uint8Register<I2C> sensor_config_;
    comms::i2c::Uint8Register<I2C> device_config_;

    

};
}
}
}

#endif // LIBFIRMWARE_DRIVERS_SENSORS_MCP9600_HPP
