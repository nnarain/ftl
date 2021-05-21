//
// timer.cpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date May 18 2021
//
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/atomic.h>

static unsigned int timer_overflow = 0;

namespace ftl
{
unsigned int timerOverflow()
{
    unsigned int data = 0;
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        data = timer_overflow;
    }
    return data;
}

void resetTimerOverflow()
{
    ATOMIC_BLOCK(ATOMIC_FORCEON)
    {
        timer_overflow = 0;
    }
}

// void enableTimerOverflowInterrupt()
// {
//     TIM
// }

} // namespace ftl


ISR(TIMER1_OVF_vect)
{
    timer_overflow++;
}
