//
// platform/avr/atmega328p/uart.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Feb 21 2021
//
#ifndef FTL_PLATFORM_AVR_ATMEGA328P_UART_HPP
#define FTL_PLATFORM_AVR_ATMEGA328P_UART_HPP

#include <ftl/platform/avr/config/uart.hpp>

namespace ftl
{
namespace platform
{
namespace avr
{
namespace atmega328p
{

UART_CONFIG(HardwareUART0, 0);

} // namespace atmega328p

} // namespace avr

} // namespace platform

} // namespace ftl


#endif // FTL_PLATFORM_AVR_ATMEGA328P_UART_HPP
