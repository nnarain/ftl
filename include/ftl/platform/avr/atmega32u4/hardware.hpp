//
// platform/avr/atmega32u4/hardware.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Feb 18 2021
//

#ifndef FTL_PLATFORM_AVR_ATMEGA32U4_HPP
#define FTL_PLATFORM_AVR_ATMEGA32U4_HPP

#include "uart.hpp"

namespace ftl
{
namespace platform
{
namespace avr
{
namespace atmega32u4
{
    /**
     * ATmega32u4 Hardware Definition
    */
    struct Hardware
    {
        using UART1 = HardwareUART1;
    };
}
}
}
}

#endif // FTL_PLATFORM_AVR_ATMEGA32U4_HPP
