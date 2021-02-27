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
#include <ftl/platform/avr/interfaces/timer.hpp>

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
        template<unsigned int PIN> using GPIOA = HardwareGPIOA<PIN>;
        template<unsigned int PIN> using GPIOB = HardwareGPIOB<PIN>;
        template<unsigned int PIN> using GPIOC = HardwareGPIOC<PIN>;
        template<unsigned int PIN> using GPIOD = HardwareGPIOD<PIN>;
        template<unsigned int PIN> using GPIOE = HardwareGPIOE<PIN>;
        template<unsigned int PIN> using GPIOF = HardwareGPIOF<PIN>;
        template<unsigned int PIN> using GPIOG = HardwareGPIOG<PIN>;
        template<unsigned int PIN> using GPIOH = HardwareGPIOH<PIN>;
        template<unsigned int PIN> using GPIOJ = HardwareGPIOJ<PIN>;
        template<unsigned int PIN> using GPIOK = HardwareGPIOK<PIN>;
        template<unsigned int PIN> using GPIOL = HardwareGPIOL<PIN>;

        /* UART */
        using UART0 = HardwareUART0;
        using UART1 = HardwareUART1;
        using UART2 = HardwareUART2;
        using UART3 = HardwareUART3;

        /* I2C / 2-Wire */
        using I2C0 = HardwareI2C;

        /* Timers */
        using Timer = AvrTimer;
    };
}
}
}
} // namespace ftl


#endif // FTL_PLATFORM_AVR_ATMEGA2560_HARDWARE_HPP
