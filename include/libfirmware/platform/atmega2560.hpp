

#ifndef LIBFIRMWARE_PLATFORM_ATMEGA2560_HPP
#define LIBFIRMWARE_PLATFORM_ATMEGA2560_HPP

#include <avr/io.h>
#include <stdint.h>

#include <libfirmware/utils/bitutil.hpp>
#include <libfirmware/comms/uart.hpp>

#include <libfirmware/platform/avr/setbaud.hpp>

namespace atmega2560
{

template<uint16_t GPIO_BASE_ADDR>
struct HardwareGpio
{
    static constexpr uint16_t INPUT_ADDR = GPIO_BASE_ADDR;
    static constexpr uint16_t DDR_ADDR = GPIO_BASE_ADDR + 1;
    static constexpr uint16_t OUTPUT_ADDR = GPIO_BASE_ADDR + 2;

    template<uint8_t PIN>
    struct OutputPin
    {
        OutputPin()
        {
            SET_BIT(_SFR_IO8(DDR_ADDR), PIN);
        }

        void set()
        {
            SET_BIT(_SFR_IO8(OUTPUT_ADDR), PIN);
        }

        void reset()
        {
            CLR_BIT(_SFR_IO8(OUTPUT_ADDR), PIN);
        }

        void toggle()
        {
            TGL_BIT(_SFR_IO8(OUTPUT_ADDR), PIN);
        }
    };

    template<uint8_t PIN>
    struct InputPin
    {
        InputPin()
        {
            CLR_BIT(_SFR_IO8(DDR_ADDR), PIN);
        }

        bool read() const
        {
            return IS_BIT_SET(_SFR_IO8(INPUT_ADDR), PIN);
        }
    };
};

using Baud = libfirmware::comms::uart::BaudRate;

template<uint16_t Addr>
struct UsartRegisters
{
    static constexpr uint16_t UCSRA = Addr;
    static constexpr uint16_t UCSRB = Addr + 0x01;
    static constexpr uint16_t UCSRC = Addr + 0x02;
    static constexpr uint16_t UBRR  = Addr + 0x04;
    static constexpr uint16_t UBRRL = Addr + 0x04;
    static constexpr uint16_t UBBRH = Addr + 0x05;
    static constexpr uint16_t UDR   = Addr + 0x06;
};

template<uint16_t IoBase>
struct HardwareUart
{
    using Regs = UsartRegisters<IoBase>;

    HardwareUart(Baud baud)
    {
        const auto uart_config = libfirmware::platform::avr::configUart9600();

        _SFR_IO16(Regs::UBRR) = uart_config.baud_value;

        if (uart_config.use_2x)
        {
            SET_BIT(_SFR_IO8(Regs::UCSRA), U2X0);
        }
        else
        {
            CLR_BIT(_SFR_IO8(Regs::UCSRA), U2X0);
        }

        // Enable RX and TX
        _SFR_IO8(Regs::UCSRB) = _BV(RXEN0) | _BV(TXEN0);

        // frame format
        _SFR_IO8(Regs::UCSRC) = _BV(UCSZ01) | _BV(UCSZ00);
    }

    void write(uint8_t data)
    {
        loop_until_bit_is_set(_SFR_IO8(Regs::UCSRA), UDRE0);
        _SFR_IO8(Regs::UDR) = data;
    }

    void write(const uint8_t* data, unsigned int size)
    {
        for (auto i = 0u; i < size; ++i)
            write(data[i]);
    }

    void write(const char* s)
    {
        if (s == nullptr) return;

        while(*s)
            write(*s++);
    }
};

struct Hardware
{
    /* General Purpose IO */
    using GpioA = HardwareGpio<0x00>;
    using GpioB = HardwareGpio<0x03>;

    /* UART */
    using Uart0 = HardwareUart<0xC0>;
    using Uart1 = HardwareUart<0xC8>;
    using Uart2 = HardwareUart<0xD0>;
    using Uart3 = HardwareUart<0x130>;

    /* I2C / 2-Wire */
};

}

#endif
