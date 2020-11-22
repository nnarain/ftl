//
// setbaud.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Oct 30 2020
//
#ifndef FTL_PLATFORM_AVR_SETBAUD_HPP
#define FTL_PLATFORM_AVR_SETBAUD_HPP

#include <avr/io.h>

namespace ftl
{
namespace platform
{
namespace avr
{
   struct BaudConfig
   {
      BaudConfig(uint16_t b, bool u) : baud_value{b}, use_2x{u} {}
      BaudConfig() : BaudConfig(0, false) {}
      uint16_t baud_value;
      bool use_2x;
   };

   static BaudConfig configUart9600()
   {
#define BAUD 9600
#include <util/setbaud.h>
         constexpr auto b = UBRR_VALUE;
         constexpr auto u = 
#if USE_2X
        true;
#else
        false;
#endif
        return BaudConfig{b, u};
#undef BAUD
   }

   static BaudConfig configUart115200()
   {
#define BAUD 115200
#include <util/setbaud.h>
         constexpr auto b = UBRR_VALUE;
         constexpr auto u = 
#if USE_2X
        true;
#else
        false;
#endif
        return BaudConfig{b, u};
#undef BAUD
   }
}
}
}

#endif
