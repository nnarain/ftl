//
// platform/avr/atmega2560/uart.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Oct 31 2020
//

#ifndef FTL_PLATFORM_AVR_ATMEGA2560_UART_HPP
#define FTL_PLATFORM_AVR_ATMEGA2560_UART_HPP

#include <ftl/comms/uart.hpp>
#include <ftl/utils/bitutil.hpp>
#include <ftl/platform/avr/utils/setbaud.hpp>

namespace ftl
{
namespace platform
{
namespace avr
{
namespace atmega2560
{
using Baud = ftl::comms::uart::BaudRate;

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
        ftl::platform::avr::BaudConfig uart_config;

        switch (baud)
        {
        case Baud::Rate_9600:
            uart_config = ftl::platform::avr::configUart9600();
            break;
        case Baud::Rate_115200:
            uart_config = ftl::platform::avr::configUart115200();
        default:
            break;
        }

        _SFR_MEM16(Regs::UBRR) = uart_config.baud_value;

        if (uart_config.use_2x)
        {
            SET_BIT(_SFR_MEM8(Regs::UCSRA), U2X0);
        }
        else
        {
            CLR_BIT(_SFR_MEM8(Regs::UCSRA), U2X0);
        }

        // // Enable RX and TX
        _SFR_MEM8(Regs::UCSRB) = _BV(RXEN0) | _BV(TXEN0);

        // // frame format
        _SFR_MEM8(Regs::UCSRC) = _BV(UCSZ01) | _BV(UCSZ00);
    }

    void write(uint8_t data)
    {
        loop_until_bit_is_set(_SFR_MEM8(Regs::UCSRA), UDRE0);
        _SFR_MEM8(Regs::UDR) = data;
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
}
}
}
} // namespace ftl

#endif
