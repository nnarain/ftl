//
// platform.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Oct 31 2020
//

// Detect AVR platforms
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega328__)
#   include <ftl/platform/avr/atmega328p/hardware.hpp>
#   define ftl_hardware_ns avr::atmega328p
#elif defined(__AVR_ATmega2560__)
#   include <ftl/platform/avr/atmega2560/hardware.hpp>
#   define ftl_hardware_ns avr::atmega2560
#elif defined(__AVR_ATmega32U4__)
#   include <ftl/platform/avr/atmega32u4/hardware.hpp>
#   define ftl_hardware_ns avr::atmega32u4
#else
# error "Could not auto detect hardware platform"
#endif

namespace ftl
{
namespace platform
{
    using Hardware = ftl_hardware_ns::Hardware;
} // namespace platform

} // namespace ftl
