//
// platform/avr/atmega2560/hardware.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Oct 31 2020
//

#ifndef FTL_PLATFORM_AVR_ATMEGA2560_HARDWARE_HPP
#define FTL_PLATFORM_AVR_ATMEGA2560_HARDWARE_HPP

#include "gpio.hpp"
#include "uart.hpp"

#include <ftl/platform/avr/interfaces/i2c.hpp>

namespace ftl
{
namespace platform
{
namespace avr
{
namespace atmega2560
{
    /**
     * ATmega2560 Hardware Definition
    */
    struct Hardware
    {
        /* General Purpose IO */
        using GpioA = HardwareGpio<0x00>;
        using GpioB = HardwareGpio<0x03>;
        using GpioC = HardwareGpio<0x06>;
        using GpioD = HardwareGpio<0x09>;

        /* UART */
        using UART0 = HardwareUART0;
        using UART1 = HardwareUART1;
        using UART2 = HardwareUART2;
        using UART3 = HardwareUART3;

        /* I2C / 2-Wire */
        using I2C = HardwareI2C;
    };
}
}
}
} // namespace ftl


#endif // FTL_PLATFORM_AVR_ATMEGA2560_HARDWARE_HPP
