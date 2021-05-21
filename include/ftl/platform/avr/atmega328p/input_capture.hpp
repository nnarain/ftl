//
// input_capture.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date May 19 2021
//

#ifndef FTL_PLATFORM_AVR_ATMEGA
#define FTL_PLATFORM_AVR_ATMEGA

#include <avr/io.h>
#include <ftl/platform/avr/interfaces/timer.hpp>

namespace ftl
{
namespace platform
{
namespace avr
{
namespace atmega328p
{
struct HardwareInputCapture1
{
    HardwareInputCapture1()
    {
        // Enable timer overflow interrupt and input capture
        TIMSK1 = (1 << TOIE1);
    }

    /**
     * \return pulse width in microseconds
     */
    unsigned int pulseIn()
    {
        // Set all bit to zero Normal operation
        TCCR1A = 0;
        // Clear Timer counter
        TCNT1 = 0;

        // Capture on rising edge, No prescaler
        TCCR1B = 0x41;
        // Clear ICP flag (Input Capture flag)
        TIFR1 = 1<<ICF1;
        // Clear Timer Overflow flag
        TIFR1 = 1<<TOV1;

        // Calculate width of Echo by Input Capture (ICP)

        // Wait for rising edge
        while ((TIFR1 & (1 << ICF1)) == 0);
        // Clear Timer counter
        TCNT1 = 0;
        // Capture on falling edge, No prescaler
        TCCR1B = 0x01;
        // Clear ICP flag (Input Capture flag)
        TIFR1 = 1<<ICF1;
        // Clear Timer Overflow flag
        TIFR1 = 1<<TOV1;
        // Clear Timer overflow count
        ftl::resetTimerOverflow();

        // Wait for falling edge
        while ((TIFR1 & (1 << ICF1)) == 0);
        // Total ticks
        const auto count = (65535 * ftl::timerOverflow()) + ICR1;

        return count / (F_CPU / 1000000UL);
    }
};
}
}
}
} // namespace ftl


#endif // FTL_PLATFORM_AVR_ATMEGA

