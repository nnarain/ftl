//
// platform/avr/atmega2560/gpio.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Oct 31 2020
//
#ifndef FTL_PLATFORM_AVR_ATMEGA2560_GPIO_HPP
#define FTL_PLATFORM_AVR_ATMEGA2560_GPIO_HPP

#include <avr/io.h>
#include <ftl/platform/avr/config/gpio.hpp>

namespace ftl
{
namespace platform
{
namespace avr
{
namespace atmega2560
{
    GPIO_CONFIG(HardwareGPIOA, A);
    GPIO_CONFIG(HardwareGPIOB, B);
    GPIO_CONFIG(HardwareGPIOC, C);
    GPIO_CONFIG(HardwareGPIOD, D);
    GPIO_CONFIG(HardwareGPIOE, E);
    GPIO_CONFIG(HardwareGPIOF, F);
    GPIO_CONFIG(HardwareGPIOG, G);
    GPIO_CONFIG(HardwareGPIOH, H);
    GPIO_CONFIG(HardwareGPIOJ, J);
    GPIO_CONFIG(HardwareGPIOK, K);
    GPIO_CONFIG(HardwareGPIOL, L);
}
}
}
} // namespace ftl

#endif // FTL_PLATFORM_AVR_ATMEGA2560_GPIO_HPP
