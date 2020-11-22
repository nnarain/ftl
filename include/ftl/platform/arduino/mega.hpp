#ifndef FTL_PLATFORM_ARDUINO_MEGA_HPP
#define FTL_PLATFORM_ARDUINO_MEGA_HPP

#include <ftl/platform/avr/atmega2560/hardware.hpp>

namespace ftl
{
namespace platform
{
namespace arduino
{
    using Baud = ftl::comms::uart::BaudRate;
    using Mega = ftl::platform::avr::atmega2560::Hardware;
}
}
}

#endif
