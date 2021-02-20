//
// platform/avr/atmega2560/uart.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Oct 31 2020
//

#ifndef FTL_PLATFORM_AVR_ATMEGA2560_UART_HPP
#define FTL_PLATFORM_AVR_ATMEGA2560_UART_HPP

#include <ftl/platform/avr/config/uart.hpp>

namespace ftl
{
namespace platform
{
namespace avr
{
namespace atmega2560
{

UART_CONFIG(HardwareUART0, 0);
UART_CONFIG(HardwareUART1, 1);
UART_CONFIG(HardwareUART2, 2);
UART_CONFIG(HardwareUART3, 3);

}
}
}
} // namespace ftl

#endif
