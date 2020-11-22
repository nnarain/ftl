//
// i2c.cpp
//
// @brief AVR I2C support library
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Nov 02 2020
//

#include <ftl/platform/avr/support/i2c.hpp>
#include <ftl/utils/bitutil.hpp>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>

// Enable I2C interrupts
#define I2C_ENABLE_ISR() SET_BIT(TWCR, TWIE)
// Disable I2C interrupts
#define I2C_DISABLE_ISR() CLR_BIT(TWCR, TWIE)
// Wait for the I2C action to complete
#define I2C_ACTION_WAIT() while(!(TWCR & BV(TWINT)))
// Trigger I2C bus action - Clear interrupt flag and set enable bit
#define I2C_ACTION_TRIGGER BV(TWINT) | BV(TWEN)
// // I2C clear interrupt flag value
// #define I2C_ClEAR_INT BV(TWINT)
// // Start condition
// #define I2C_START_CONDITION BV(TWSTA)
// // Stop condition
// #define I2C_STOP_CONDITION BV(TWSTO)
// // I2C

// TODO Remove
#define SCL 100000UL

namespace ftl
{
namespace platform
{
namespace avr
{
namespace i2c
{
    void init(ClockMode clock)
    {
        // SCL = F_CPU / (16 + 2(TWBR) * (4^TWPS))
        // TWBR = ((F_CPU / SCL) - 16) / 2 * (4^TWPS)

        // Will leave prescaler bit as defaults, TWPS, at 0 -> prescaler of 1
        // TODO(nnarain): Prescaler config

        // Calculate I2C baud rate
        const auto scl_freq = static_cast<unsigned long>(clock);
        TWBR = ((F_CPU / scl_freq) - 16) / 2;

        TWCR = BV(TWEN);

        CLR_BIT(TWSR, 0);
        CLR_BIT(TWSR, 1);

        // TODO(nnarain): Interrupt driven?
        // I2C_ENABLE_ISR();
    }

    void start()
    {
        // Send start condition
        // Clear interrupt, I2C enable and start flag
        TWCR = I2C_ACTION_TRIGGER | BV(TWSTA);
        I2C_ACTION_WAIT();
    }

    uint8_t begin(uint8_t address, SlaMode mode)
    {
        start();

        uint8_t status = TW_STATUS;

        // Should be in a start state. Return early otherwise
        if (status != TW_START && status != TW_REP_START)
        {
            return status;
        }

        // Send the slave address and mode on the bus
        write((address << 1) | static_cast<uint8_t>(mode));

        status = TW_STATUS;
        if (mode == SlaMode::Read && status != TW_MR_SLA_ACK)
        {
            return status;
        }
        else if (mode == SlaMode::Write && status != TW_MT_SLA_ACK)
        {
            return status;
        }

        return 0;
    }

    void end()
    {
        stop();
    }

    void stop()
    {
        // Send the stop condition
        // Clear interrupt, I2C enable, stop flag
        TWCR = I2C_ACTION_TRIGGER | BV(TWSTO);

        loop_until_bit_is_clear(TWCR, TWSTO);
        // I2C_ACTION_WAIT();
    }

    void write(uint8_t data)
    {
        // Load the data register
        TWDR = data;
        // Trigger transfer
        TWCR = I2C_ACTION_TRIGGER;

        I2C_ACTION_WAIT();
    }

    void write(const uint8_t* data, uint32_t len)
    {
        for (auto i = 0u; i < len; ++i)
        {
            write(data[i]);
        }
    }

    uint8_t read(bool ack)
    {
        uint8_t trigger = I2C_ACTION_TRIGGER;
        // Set whether a ACK should be generated
        if (ack)
        {
            trigger |= BV(TWEA);
        }

        TWCR = trigger;
        I2C_ACTION_WAIT();

        return TWDR;
    }

    void read(uint8_t* data, uint32_t len)
    {
        for (auto i = 0u; i < len; ++i)
        {
            // Read the bus and generate a NACK on the last read
            data[i] = read(i < (len - 1));
        }
    }

    comms::i2c::State status()
    {
        switch (TW_STATUS)
        {
        case TW_START:
            return comms::i2c::State::Start;
        case TW_REP_START:
            return comms::i2c::State::RepeatedStart;
        case TW_MT_SLA_ACK:
            return comms::i2c::State::MT_SlaveAck;
        case TW_MT_SLA_NACK:
            return comms::i2c::State::MT_DataNAck;
        case TW_MT_DATA_ACK:
            return comms::i2c::State::MT_DataAck;
        case TW_MT_DATA_NACK:
            return comms::i2c::State::MT_DataNAck;
        default:
            // FIXME: All I2C states
            return comms::i2c::State::Ready;
        }
    }

    const char* statusToStr(uint8_t status)
    {
        switch (status)
        {
        case 0x08:
            return "start condition transmitted";
        case 0x10:
            return "repeated start condition transmitted";
        case 0x18:
            return "SLA+W transmitted, ACK received";
        case 0x20:
            return "SLA+W transmitted, NACK received";
        case 0x28:
            return "Data transmitted, ACK received";
        case 0x30:
            return "data transmitted, NACK received";
        case 0x38:
            return "arbitration lost in SLA+R/W or NACK/data";
        case 0x40:
            return "SLA+R transmitted, ACK received";
        case 0x48:
            return "SLA+R transmitted, NACK received";
        case 0x50:
            return "Data received, ACK returned";
        case 0x58:
            return "Data received, NACK returned";
        case 0xA8:
            return "SLA+R received, ACK returned";
        case 0xB0:
            return "arbitration lost in SLA+RW, SLA+R received, ACK returned";
        case 0xB8:
            return "data transmitted, ACK received";
        case 0xC0:
            return "data transmitted, NACK received";
        case 0xC8:
            return "last data byte transmitted, ACK received";
        case 0x60:
            return "SLA+W received, ACK returned";
        case 0x68:
            return "arbitration lost in SLA+RW, SLA+W received, ACK returned";
        case 0x70:
            return "general call received, ACK returned";
        case 0x78:
            return "arbitration lost in SLA+RW, general call received, ACK returned";
        case 0x80:
            return "data received, ACK returned";
        case 0x88:
            return "data received, NACK returned";
        case 0x90:
            return "general call data received, ACK returned";
        case 0x98:
            return "general call data received, NACK returned";
        case 0xA0:
            return "stop or repeated start condition received while selected";
        case 0xF8:
            return "no state information available";
        case 0x00:
            return "illegal start or stop condition";
        default:
            return "Invalid status";
        }
    }
}
}
}
} // namespace ftl

// I2C Interrupt Service Routine
ISR(TWI_vect)
{

}
