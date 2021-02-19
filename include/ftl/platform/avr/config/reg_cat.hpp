//
// platform/avr/config/reg_cat.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Feb 20 2021
//

#ifndef FTL_PLATFORM_AVR_CONFIG_REG_CAT_HPP
#define FTL_PLATFORM_AVR_CONFIG_REG_CAT_HPP

#define AVR_CAT1(base, i) base ## i
#define AVR_CAT2(base, i, suf) base ## i ## suf

#endif // FTL_PLATFORM_AVR_CONFIG_REG_CAT_HPP
