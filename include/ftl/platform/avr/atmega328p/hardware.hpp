//
// platform/avr/atmega328p/hardware.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Feb 21 2021
//
#ifndef FTL_PLATFORM_AVR_ATMEGA328P_HARDWARE_HPP
#define FTL_PLATFORM_AVR_ATMEGA328P_HARDWARE_HPP

#include "uart.hpp"

#include <ftl/platform/avr/interfaces/i2c.hpp>

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
        /* UART */
        using UART0 = HardwareUART0;

        /* I2C / 2-Wire */
        using I2C0 = HardwareI2C;
    };

} // namespace atmega328p

} // namespace avr

} // namespace platform

} // namespace ftl


#endif // FTL_PLATFORM_AVR_ATMEGA328P_HARDWARE_HPP
