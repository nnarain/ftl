//
// gpio.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date May 18 2021
//
#ifndef FTL_PLATFORM_AVR_ATMEGA328P_GPIO_HPP
#define FTL_PLATFORM_AVR_ATMEGA328P_GPIO_HPP

#include <avr/io.h>
#include <ftl/platform/avr/config/gpio.hpp>

namespace ftl
{
namespace platform
{
namespace avr
{
namespace atmega328p
{
    GPIO_CONFIG(HardwareGPIOB, B);
    GPIO_CONFIG(HardwareGPIOC, C);
    GPIO_CONFIG(HardwareGPIOD, D);
}
}
}
} // namespace ftl

#endif // FTL_PLATFORM_AVR_ATMEGA328P_GPIO_HPP