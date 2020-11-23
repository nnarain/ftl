//
// mcp9808.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 22 2020
//

#ifndef FTL_DRIVERS_SENSORS_MCP9808_HPP
#define FTL_DRIVERS_SENSORS_MCP9808_HPP

#include <ftl/comms/i2c/i2c_device.hpp>
#include <ftl/comms/i2c/i2c_register.hpp>

#include <ftl/utils/bitutil.hpp>

#include <stdint.h>

namespace ftl
{
namespace drivers
{
namespace sensors
{
    enum class Mcp9808_Hysteresis
    {
        // 0 C
        Limit_0 = 0x00,
        // 1.5 C
        Limit_1_5 = 0x01,
        // 3.0 C
        Limit_3_0 = 0x02,
        // 6.0 C
        Limit_6_0 = 0x03,
    };

    template<class I2C>
    class Mcp9808
    {
        static constexpr uint8_t CONFIG_REGISTER = 0x01;
        static constexpr uint8_t ALERT_UPPER_BOUND_REGISTER = 0x02;
        static constexpr uint8_t ALERT_LOWER_BOUND_REGISTER = 0x03;
        static constexpr uint8_t CRITICAL_LIMIT_REGISTER = 0x04;
        static constexpr uint8_t TEMPERATURE_REGISTER = 0x05;
        static constexpr uint8_t MANUFACTURER_ID_REGISTER = 0x06;
        static constexpr uint8_t DEVICE_ID_REGISTER = 0x07;
        static constexpr uint8_t RESOLUTION_REGISTER = 0x08;

        static constexpr uint16_t MANUFACTURER_ID = 0x0054;
        static constexpr uint8_t DEVICE_ID = 0x04;

    public:
        Mcp9808(uint8_t address)
            : device_{address}
        {
        }

        /**
         * Verify device manufacturer ID and device ID
        */
        bool verify()
        {
            const auto device_id = getDeviceId();
            const auto manufacturer_id = getManufacturerId();

            return device_id == DEVICE_ID && manufacturer_id == MANUFACTURER_ID;
        }

        /**
         * Enable or disable device
         * 
         * Disabling puts the device into shutdown mode
        */
        void enable(bool enabled)
        {
            comms::i2c::Uint16Register<I2C> config{device_, CONFIG_REGISTER};
            // Write 1 to shutdown (NOT enabled)
            config.writeBits(static_cast<uint16_t>(!enabled), 0x01, 8);
        }

        /**
         * Check if the device is shutdown
        */
        bool isShutdown()
        {
            comms::i2c::Uint16Register<I2C> config{device_, CONFIG_REGISTER};
            return config.readBits(0x01, 8) == 1;
        }

        /**
         * Set hysteresis for temperature limits
         * 
         * Cannot be altered when locked
         * Can be set in shutdown mode
        */
        void setHysteresisLimit(Mcp9808_Hysteresis hyst)
        {
            comms::i2c::Uint16Register<I2C> config{device_, CONFIG_REGISTER};
            config.writeBits(static_cast<uint16_t>(hyst), 0x03, 9);
        }

        /**
         * Get hysteresis limit
        */
        Mcp9808_Hysteresis getHysteresis()
        {
            comms::i2c::Uint16Register<I2C> config{device_, CONFIG_REGISTER};
            return static_cast<Mcp9808_Hysteresis>(config.readBits(0x03, 9));
        }

        /**
         * Lock critical temperature register
        */
        void setCriticalLock(bool lock)
        {
            comms::i2c::Uint16Register<I2C> config{device_, CONFIG_REGISTER};
            config.writeBits(lock, 0x01, 7);
        }

        /**
         * Get critical temperature register
        */
        bool getCriticalLock()
        {
            comms::i2c::Uint16Register<I2C> config{device_, CONFIG_REGISTER};
            return config.readBits(0x01, 7);
        }

        /**
         * Set window lock (hysteresis bits)
        */
        void setWindowLock(bool lock)
        {
            comms::i2c::Uint16Register<I2C> config{device_, CONFIG_REGISTER};
            config.writeBits(lock, 0x01, 6);
        }

        /**
         * Get window lock bit
        */
        bool getWindowLock()
        {
            comms::i2c::Uint16Register<I2C> config{device_, CONFIG_REGISTER};
            return config.readBits(0x01, 6);
        }

        /**
         * Set interrupt clear bit
        */
        void setInterruptClearBit(bool bit)
        {
            comms::i2c::Uint16Register<I2C> config{device_, CONFIG_REGISTER};
            config.writeBits(bit, 0x01, 5);
        }

        /**
         * Get interrupt clear bit (Reading sets the bit to zero)
        */
        bool getInterruptClearBit()
        {
            comms::i2c::Uint16Register<I2C> config{device_, CONFIG_REGISTER};
            return config.readBits(0x01, 5);
        }

        /**
         * Get alert status
        */
        bool alert()
        {
            comms::i2c::Uint16Register<I2C> config{device_, CONFIG_REGISTER};
            return config.readBits(0x01, 4);
        }

        /**
         * Set alert count
        */
        void setAlertCount(bool enabled)
        {
            comms::i2c::Uint16Register<I2C> config{device_, CONFIG_REGISTER};
            config.writeBits(enabled, 0x01, 3);
        }

        /**
         * Get Alert count enable
        */
        bool getAlertCount()
        {
            comms::i2c::Uint16Register<I2C> config{device_, CONFIG_REGISTER};
            return config.readBits(0x01, 3);
        }

        /**
         * Set alert select. If true, alert is set when ambient temperature is greater than the critical
         * threshold. Other set for upper, lower and critical thresholds
         * TODO: Mode enum?
        */
        void setAlertSelect(bool enable)
        {
            comms::i2c::Uint16Register<I2C> config{device_, CONFIG_REGISTER};
            config.writeBits(enable, 0x01, 2);
        }

        bool getAlertSelect()
        {
            comms::i2c::Uint16Register<I2C> config{device_, CONFIG_REGISTER};
            return config.readBits(0x01, 2);
        }

        /**
         * Set alert output polarity
        */
        void setAlertActiveHigh(bool enable)
        {
            comms::i2c::Uint16Register<I2C> config{device_, CONFIG_REGISTER};
            config.writeBits(enable, 0x01, 1);
        }

        bool getAlertActiveHigh()
        {
            comms::i2c::Uint16Register<I2C> config{device_, CONFIG_REGISTER};
            return config.readBits(0x01, 1);
        }

        /**
         * Set interrupt mode
        */
        void setAlertInterruptMode(bool enabled)
        {
            comms::i2c::Uint16Register<I2C> config{device_, CONFIG_REGISTER};
            config.writeBits(enabled, 0x01, 0);
        }

        bool getAlertInterruptMode()
        {
            comms::i2c::Uint16Register<I2C> config{device_, CONFIG_REGISTER};
            return config.readBits(0x01, 0);
        }

        /**
         * Set upper bound register
        */
        void setUpperTemperatureLimit(float t)
        {
            writeTemperatureRegister(ALERT_UPPER_BOUND_REGISTER, t);
        }

        float getUpperTemperatureLimit()
        {
            return readTemperatureRegister(ALERT_UPPER_BOUND_REGISTER);
        }

        /**
         * Set lower bound register
        */
        void setLowerTemperatureLimit(float t)
        {
            writeTemperatureRegister(ALERT_LOWER_BOUND_REGISTER, t);
        }

        float getLowerTemperatureLimit()
        {
            return readTemperatureRegister(ALERT_LOWER_BOUND_REGISTER);
        }

        /**
         * Set critical bound register
        */
        void setCriticalTemperatureLimit(float t)
        {
            writeTemperatureRegister(CRITICAL_LIMIT_REGISTER, t);
        }

        float getCriticalTemperatureLimit()
        {
            return readTemperatureRegister(CRITICAL_LIMIT_REGISTER);
        }

        float getAmbientTemperature()
        {
            comms::i2c::Uint16Register<I2C> temperature{device_, TEMPERATURE_REGISTER};

            float temp = 0.0f;
            uint16_t t = temperature.read();

            // Assigned the first 12 bites
            temp = t & 0x0FFF;
            // Shift the decimal portion down
            temp /= 16.0;
            // If negative, subtract 256
            if (t & 0x1000)
                temp -= 256;

            return temp;
        }

        /**
         * Get critical alert status
        */
        bool alertCritical()
        {
            return readAlertSource() & 0x04;
        }

        /**
         * Get upper limit alert source
        */
        bool alertUpper()
        {
            return readAlertSource() & 0x02;
        }

        /**
         * Get lower limit alert source
        */
        bool alertLower()
        {
            return readAlertSource() & 0x01;
        }

        /**
         * Get manufacturer ID
        */
        uint16_t getManufacturerId()
        {
            comms::i2c::Uint16Register<I2C> id{device_, MANUFACTURER_ID_REGISTER};
            return id.read();
        }

        uint8_t getDeviceId()
        {
            comms::i2c::Uint16Register<I2C> id{device_, DEVICE_ID_REGISTER};
            return id.read() >> 8;
        }

        uint8_t getDeviceRevision()
        {
            comms::i2c::Uint16Register<I2C> id{device_, DEVICE_ID_REGISTER};
            return id.read() & 0x00FF;
        }

    private:
        /**
         * Write a temperature value to the 16 bit register
        */
        void writeTemperatureRegister(uint8_t pointer, float temp)
        {
            // Convert float to signed 16-bit number
            int16_t t = static_cast<int16_t>(temp);
            // Write to the register
            comms::i2c::Int16Register<I2C> reg{device_, pointer};
            reg.write(t);
        }

        /**
         * Read temperature register as float
        */
        float readTemperatureRegister(uint8_t pointer)
        {
            comms::i2c::Int16Register<I2C> reg{device_, pointer};
            int16_t temp = reg.read();

            // Convert 12-bit signed to 16-bit signed
            // TODO: Make this generic?
            if (temp & 0x0800)
            {
                temp = (static_cast<int16_t>(-1) ^ 0xFFF) | temp;
            }

            return static_cast<float>(temp);
        }

        uint8_t readAlertSource()
        {
            comms::i2c::Uint16Register<I2C> temperature{device_, TEMPERATURE_REGISTER};
            uint16_t data = temperature.readBits(0x07, 13);
            return data;
        }

        comms::i2c::I2CDevice<I2C> device_;
    };
}
}
}

#endif
