//
// platform/avr/atmega328p/hardware.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Feb 21 2021
//
#ifndef FTL_PLATFORM_AVR_ATMEGA328P_HARDWARE_HPP
#define FTL_PLATFORM_AVR_ATMEGA328P_HARDWARE_HPP

#include "uart.hpp"
#include "gpio.hpp"
#include "input_capture.hpp"

#include <ftl/platform/avr/interfaces/i2c.hpp>
#include <ftl/platform/avr/interfaces/timer.hpp>

namespace ftl
{
namespace platform
{
namespace avr
{
namespace atmega328p
{

    /**
     * ATmega328p Hardware Definition
    */
    struct Hardware
    {
        /* General Purpose IO */
        template<unsigned int PIN> using GPIOB = HardwareGPIOB<PIN>;
        template<unsigned int PIN> using GPIOC = HardwareGPIOC<PIN>;
        template<unsigned int PIN> using GPIOD = HardwareGPIOD<PIN>;

        /* UART */
        using UART0 = HardwareUART0;

        /* I2C / 2-Wire */
        using I2C0 = HardwareI2C;

        /* Timer */
        using Timer = AvrTimer;

        /* Input capture */
        using InputCapture1 = HardwareInputCapture1;
    };

} // namespace atmega328p

} // namespace avr

} // namespace platform

} // namespace ftl


#endif // FTL_PLATFORM_AVR_ATMEGA328P_HARDWARE_HPP
