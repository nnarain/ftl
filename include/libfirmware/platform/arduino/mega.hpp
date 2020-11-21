#ifndef LIBFIRMWARE_PLATFORM_ARDUINO_MEGA_HPP
#define LIBFIRMWARE_PLATFORM_ARDUINO_MEGA_HPP

#include <libfirmware/platform/avr/atmega2560/hardware.hpp>

namespace libfirmware
{
namespace platform
{
namespace arduino
{
    using Baud = libfirmware::comms::uart::BaudRate;
    using Mega = libfirmware::platform::avr::atmega2560::Hardware;
}
}
}

#endif
