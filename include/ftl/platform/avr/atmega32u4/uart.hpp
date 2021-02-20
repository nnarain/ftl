//
// platform/avr/atmega32u4/uart.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Feb 20 2021
//

#ifndef FTL_PLATFORM_AVR_ATMEGA32U4_UART_HPP
#define FTL_PLATFORM_AVR_ATMEGA32U4_UART_HPP

#include <ftl/platform/avr/config/uart.hpp>

namespace ftl
{
namespace platform
{
namespace avr
{
namespace atmega32u4
{

UART_CONFIG(HardwareUART1, 1);

} // namespace atmega32u4

} // namespace avr

} // namespace platform

} // namespace ftl

#endif // FTL_PLATFORM_AVR_ATMEGA32U4_UART_HPP
