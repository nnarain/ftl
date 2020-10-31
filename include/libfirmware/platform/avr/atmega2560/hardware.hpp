//
// platform/avr/atmega2560/hardware.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Oct 31 2020
//

#ifndef LIBFIRMWARE_PLATFORM_AVR_ATMEGA2560_HARDWARE_HPP
#define LIBFIRMWARE_PLATFORM_AVR_ATMEGA2560_HARDWARE_HPP

#include "uart.hpp"
#include "gpio.hpp"

namespace libfirmware
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

        /* UART */
        using Uart0 = HardwareUart<0xC0>;
        using Uart1 = HardwareUart<0xC8>;
        using Uart2 = HardwareUart<0xD0>;
        using Uart3 = HardwareUart<0x130>;

        /* I2C / 2-Wire */
    };
}
}
}
} // namespace libfirmware


#endif // LIBFIRMWARE_PLATFORM_AVR_ATMEGA2560_HARDWARE_HPP
