//
// platform/avr/config/gpio.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Feb 26 2021
//
#ifndef FTL_PLATFORM_AVR_CONFIG_GPIO_HPP
#define FTL_PLATFORM_AVR_CONFIG_GPIO_HPP

#include <ftl/gpio/gpio.hpp>
#include <ftl/utils/bitutil.hpp>
#include "reg_cat.hpp"

#define GPIO_CONFIG(name, port) \
    template<unsigned int pin> \
    struct name \
    { \
        name(ftl::GpioState state) \
        { \
            if (state == ftl::GpioState::Output) \
            { \
                SET_BIT(AVR_CAT1(DDR, port), pin); \
            } \
            else \
            { \
                CLR_BIT(AVR_CAT1(DDR, port), pin); \
            } \
        } \
        void set() \
        { \
            SET_BIT(AVR_CAT1(PORT, port), pin); \
        } \
        void reset() \
        { \
            CLR_BIT(AVR_CAT1(PORT, port), pin); \
        } \
        void toggle() \
        { \
            TGL_BIT(AVR_CAT1(PORT, port), pin); \
        } \
        bool read() \
        { \
            return IS_BIT_SET(AVR_CAT1(PIN, port), pin); \
        } \
    }

#endif // FTL_PLATFORM_AVR_CONFIG_GPIO_HPP
