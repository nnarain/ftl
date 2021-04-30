//
// pca9685.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Apr 25 2021
//

#ifndef FTL_DRIVERS_PWM_PCA9685_HPP
#define FTL_DRIVERS_PWM_PCA9685_HPP

#include <ftl/comms/i2c/i2c_device.hpp>
#include <ftl/comms/i2c/i2c_register.hpp>

namespace ftl
{
namespace drivers
{
/**
 * 16 channel PWM controller over I2C
 * 
 * * Each channel has 12-bit resolution (4096 steps)
 * * Operating frequency: 24 Hz - 1526 Hz
 * * Duty cycle: 0% - 100%
 * 
 * \tparam I2C Host device I2C (2-wire) interface
 */
template<typename I2C>
class Pca9685
{
    static constexpr uint8_t MODE1_REGISTER = 0x00;
    static constexpr uint8_t MODE1_REGISTER_SLEEP_BIT = 4;
    static constexpr uint8_t MODE1_REGISTER_AI_BIT = 5;

    static constexpr uint8_t MODE2_REGISTER = 0x01;
    static constexpr uint8_t MODE2_REGISTER_INVERT_BIT = 4;
    static constexpr uint8_t MODE2_REGISTER_OUTPUT_CHANGE_BIT = 3;
    static constexpr uint8_t MODE2_REGISTER_OUTPUT_DRIVE_BIT = 2;

    static constexpr uint8_t LED_REGISTER_BASE = 0x06;

    static constexpr uint8_t PRESCALE_REGISTER = 0xFE;

    static constexpr uint16_t MAX_STEPS = 4096;
    static constexpr float BUILTIN_OSC_CLOCK_FREQ_HZ = 25000000.0f;

public:
    Pca9685(uint8_t addr, float ext_osc_freq)
        : device_{addr}
        , osc_freq_{ext_osc_freq}
    {
    }

    Pca9685(uint8_t addr) : Pca9685{addr, BUILTIN_OSC_CLOCK_FREQ_HZ}
    {
    }

    ~Pca9685() = default;

    bool initialize()
    {
        if (!device_.detect())
        {
            return false;
        }

        // enable(true);
        enableAutoIncrement(true);

        return true;
    }

    /**
     * Enable or disable the device
     * 
     * Disabling the device will put it into sleep mode
     */
    void enable(bool e)
    {
        comms::i2c::Uint8Register<I2C> mode1{device_, MODE1_REGISTER};
        // We are actually setting the sleep register which is the opposite of "enable"
        mode1.writeBit(!e, MODE1_REGISTER_SLEEP_BIT);
    }

    /**
     * Read the sleep status of the device
     */
    bool getSleep()
    {
        comms::i2c::Uint8Register<I2C> mode1{device_, MODE1_REGISTER};
        return mode1.readBit(MODE1_REGISTER_SLEEP_BIT);
    }
    /**
     * Enable or disable auto incrementing mode for addressing registers
     */
    void enableAutoIncrement(bool ai)
    {
        comms::i2c::Uint8Register<I2C> mode1{device_, MODE1_REGISTER};
        mode1.writeBit(ai, MODE1_REGISTER_AI_BIT);
    }

    /**
     * Get auto increment status
     */
    bool getAutoIncrement()
    {
        comms::i2c::Uint8Register<I2C> mode1{device_, MODE1_REGISTER};
        return mode1.readBit(MODE1_REGISTER_AI_BIT);
    }

    /**
     * Invert output
     */
    void invert(bool invert)
    {
        comms::i2c::Uint8Register<I2C> mode2{device_, MODE2_REGISTER};
        mode2.writeBit(invert, MODE2_REGISTER_INVERT_BIT);
    }

    /**
     * Get invert status
     */
    bool getInvert()
    {
        comms::i2c::Uint8Register<I2C> mode2{device_, MODE2_REGISTER};
        return mode2.readBit(MODE2_REGISTER_INVERT_BIT);
    }

    /**
     * Specify when the output change occurs. If 'true', output changes on ACK, else on STOP
     */
    void setOutputChange(bool och)
    {
        comms::i2c::Uint8Register<I2C> mode2{device_, MODE2_REGISTER};
        mode2.writeBit(och, MODE2_REGISTER_OUTPUT_CHANGE_BIT);
    }

    void setOutputChangeStop()
    {
        setOutputChange(false);
    }

    void setOutputChangeAck()
    {
        setOutputChange(true);
    }

    /**
     * Get output change
     */
    bool getOutputChange()
    {
        comms::i2c::Uint8Register<I2C> mode2{device_, MODE2_REGISTER};
        return mode2.readBit(MODE2_REGISTER_OUTPUT_CHANGE_BIT);
    }

    /**
     * Set output drive type. True for totem pole structure, false for open drain
     */
    void setOutputDrive(bool output_drive)
    {
        comms::i2c::Uint8Register<I2C> mode2{device_, MODE2_REGISTER};
        mode2.writeBit(output_drive, MODE2_REGISTER_OUTPUT_DRIVE_BIT);
    }

    void setOutputDriveTotemPole()
    {
        setOutputDrive(true);
    }

    void setOutputDriveOpenDrain()
    {
        setOutputDrive(false);
    }

    /**
     * Get output drive type
     */
    bool getOutputDriveType()
    {
        comms::i2c::Uint8Register<I2C> mode2{device_, MODE2_REGISTER};
        return mode2.readBit(MODE2_REGISTER_OUTPUT_DRIVE_BIT);
    }

    /**
     * Set PWM frequency
     * 
     * Note: The device MUST be put into sleep mode before setting the frequency
     * Note: Frequency range [24 Hz, 1526 Hz]
     */
    void setFrequency(float freq)
    {
        if (freq < 24.0f || freq > 1526.0f) return;

        const float prescale = (osc_freq_ / static_cast<float>(MAX_STEPS) / freq) - 1.0f;
        const auto value = static_cast<uint8_t>(prescale);

        comms::i2c::Uint8Register<I2C> prescale_register{device_, PRESCALE_REGISTER};
        prescale_register.write(value);
    }

    /**
     * Get the value stored in the prescale register
     */
    uint8_t getPrescale()
    {
        comms::i2c::Uint8Register<I2C> prescale_register{device_, PRESCALE_REGISTER};
        return prescale_register.read();
    }

    /**
     * Set the desired duty cycle on the specified channel. This sets the start time to 0 (immediate)
     */
    void setDutyCycle(uint8_t channel, float duty)
    {
        if (duty > 1.0)
        {
            duty = 1.0;
        }

        const auto steps = (static_cast<float>(MAX_STEPS) - 1) * duty;
        setPWM(channel, 0, steps);
    }

    /**
     * Set the ON and OFF values in their corresponding registers
     */
    void setPWM(uint8_t channel, uint16_t on, uint16_t off)
    {
        const auto base = LED_REGISTER_BASE + (4 * channel);

        if (device_.begin(comms::i2c::SlaMode::Write))
        {
            // Set the register pointer
            device_.write(base);
            // This driver leverages auto-increment to write to the pwm registers
            device_.write(on & 0xFF);
            device_.write(on >> 8);
            device_.write(off & 0xFF);
            device_.write(off >> 8);
            device_.end();
        }
    }

    /**
     * Get the value stored in the ON register of the specifed channel
     */
    uint16_t getChannelOn(uint8_t channel)
    {
        const auto reg = LED_REGISTER_BASE + (4 * channel);
        comms::i2c::Uint16Register<I2C, comms::i2c::Endian::Little> led_on{device_, reg};

        return led_on.read();
    }

    /**
     * Get the value stored in the OFF register of the specifed channel
     */
    uint16_t getChannelOff(uint8_t channel)
    {
        const auto reg = (LED_REGISTER_BASE + 2) + (4 * channel);
        comms::i2c::Uint16Register<I2C, comms::i2c::Endian::Little> led_off{device_, reg};

        return led_off.read();
    }

    /**
     * Set always ON/OFF bit
     */
    void setState(uint8_t channel, bool state)
    {
        if (state)
        {
            // Sets the 4th bit in the LED_ON_H register
            setPWM(channel, 4096, 0);
        }
        else
        {
            // Clears the 4th bit in the LED_ON_H register
            setPWM(channel, 0, 0);
        }
    }

private:
    comms::i2c::I2CDevice<I2C> device_;
    // Oscillator frequency
    float osc_freq_;
};
}
}

#endif // FTL_DRIVERS_PWM_PCA9685_HPP
