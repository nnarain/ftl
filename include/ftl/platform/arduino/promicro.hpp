//
// platforms/arduino/promicro.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Feb 20 2021
//

#ifndef FTL_PLATFORM_ARDUINO_PROMICRO_HPP
#define FTL_PLATFORM_ARDUINO_PROMICRO_HPP

#include <ftl/platform/avr/atmega32u4/hardware.hpp>

namespace ftl
{
namespace platform
{
namespace arduino
{
    using Baud = ftl::comms::uart::BaudRate;
    using ProMicro = ftl::platform::avr::atmega32u4::Hardware;
}
}
}

#endif // FTL_PLATFORM_ARDUINO_PROMICRO_HPP
