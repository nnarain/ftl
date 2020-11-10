//
// platform/avr/atmega2560/gpio.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Oct 31 2020
//
#ifndef LIBFIRMWARE_PLATFORM_AVR_ATMEGA2560_GPIO_HPP
#define LIBFIRMWARE_PLATFORM_AVR_ATMEGA2560_GPIO_HPP

#include <stdint.h>

#include <libfirmware/utils/bitutil.hpp>

namespace libfirmware
{
namespace platform
{
namespace avr
{
namespace atmega2560
{
    template<uint16_t GPIO_BASE_ADDR>
    struct HardwareGpio
    {
        static constexpr uint16_t INPUT_ADDR = GPIO_BASE_ADDR;
        static constexpr uint16_t DDR_ADDR = GPIO_BASE_ADDR + 1;
        static constexpr uint16_t OUTPUT_ADDR = GPIO_BASE_ADDR + 2;

        template<uint8_t PIN>
        struct OutputPin
        {
            OutputPin()
            {
                SET_BIT(_SFR_IO8(DDR_ADDR), PIN);
            }

            void set()
            {
                SET_BIT(_SFR_IO8(OUTPUT_ADDR), PIN);
            }

            void reset()
            {
                CLR_BIT(_SFR_IO8(OUTPUT_ADDR), PIN);
            }

            void toggle()
            {
                TGL_BIT(_SFR_IO8(OUTPUT_ADDR), PIN);
            }
        };

        template<uint8_t PIN>
        struct InputPin
        {
            InputPin()
            {
                CLR_BIT(_SFR_IO8(DDR_ADDR), PIN);
            }

            bool read() const
            {
                return IS_BIT_SET(_SFR_IO8(INPUT_ADDR), PIN);
            }
        };
    };
}
}
}
} // namespace libfirmware

#endif // LIBFIRMWARE_PLATFORM_AVR_ATMEGA2560_GPIO_HPP
