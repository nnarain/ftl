//
// platform/avr/atmega32u4/hardware.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Feb 18 2021
//

#ifndef FTL_PLATFORM_AVR_ATMEGA32U4_HPP
#define FTL_PLATFORM_AVR_ATMEGA32U4_HPP

#include "uart.hpp"
#include <ftl/platform/avr/interfaces/i2c.hpp>
#include <ftl/platform/avr/interfaces/timer.hpp>

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
        /* UART */
        using UART1 = HardwareUART1;

        /* I2C / 2-wire */
        using I2C0 = HardwareI2C;

        /* Timers */
        using Timer = AvrTimer;
    };
}
}
}
}

#endif // FTL_PLATFORM_AVR_ATMEGA32U4_HPP
