//
// Macros for building AVR UART Interfaces
//
// platform/avr/config/uart.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Feb 20 2021
//

#ifndef FTL_PLATFORM_AVR_CONFIG_UART_HPP
#define FTL_PLATFORM_AVR_CONFIG_UART_HPP

#include <ftl/utils/bitutil.hpp>
#include <ftl/platform/avr/utils/setbaud.hpp>

#include "reg_cat.hpp"

#define UART_CONFIG(NAME, i) \
    struct NAME \
    { \
        NAME(ftl::comms::uart::BaudRate baud) \
        { \
            ftl::platform::avr::BaudConfig uart_config; \
\
            switch(baud) \
            { \
            case ftl::comms::uart::BaudRate::Rate_9600: \
                uart_config = ftl::platform::avr::configUart9600(); \
                break; \
            case ftl::comms::uart::BaudRate::Rate_115200: \
                uart_config = ftl::platform::avr::configUart115200(); \
                break; \
            } \
\
            AVR_CAT1(UBRR, i) = uart_config.baud_value; \
\
            if (uart_config.use_2x) \
            { \
                SET_BIT(AVR_CAT2(UCSR, i, A), AVR_CAT1(U2X, i)); \
            } \
            else \
            { \
                CLR_BIT(AVR_CAT2(UCSR, i, A), AVR_CAT1(U2X, i)); \
            } \
\
            AVR_CAT2(UCSR, i, B) = _BV(AVR_CAT1(RXEN, i)) | _BV(AVR_CAT1(TXEN, i)); \
            AVR_CAT2(UCSR, i, C) = _BV(AVR_CAT2(UCSZ, i, 1)) | _BV(AVR_CAT2(UCSZ, i, 0)); \
        } \
\
        void write(uint8_t data) \
        { \
            loop_until_bit_is_set(AVR_CAT2(UCSR, i, A), AVR_CAT1(UDRE, i)); \
            AVR_CAT1(UDR, i) = data; \
        } \
\
        void write(const uint8_t* data, unsigned int size) \
        { \
            for (auto j = 0u; j < size; ++j) \
                write(data[j]); \
        } \
\
        void write(const char* s) \
        { \
            if (s == nullptr) return; \
            while(*s) \
                write(*s++); \
        } \
    }

#endif // FTL_PLATFORM_AVR_CONFIG_UART_HPP
