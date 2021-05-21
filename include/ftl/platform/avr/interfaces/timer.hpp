//
// platform/avr/timer.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Feb 24 2021
//

#ifndef FTL_PLATFORM_AVR_TIMER_HPP
#define FTL_PLATFORM_AVR_TIMER_HPP

#include <util/delay.h>

namespace ftl
{
unsigned int timerOverflow();
void resetTimerOverflow();
// void enableTimerOverflowInterrupt();

namespace platform
{
namespace avr
{

/**
 * AVR generic timer interface
*/
struct AvrTimer
{
    static void delayMs(unsigned int ms)
    {
        _delay_ms(ms);
    }

    static void delayUs(unsigned int us)
    {
        _delay_us(us);
    }
};

} // namespace avr

} // namespace platform

} // namespace ftl


#endif // FTL_PLATFORM_AVR_TIMER_HPP
