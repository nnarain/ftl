//
// logger.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Oct 31 2020
//

#ifndef LIBFIRMWARE_UTILS_LOGGER_HPP
#define LIBFIRMWARE_UTILS_LOGGER_HPP

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

namespace libfirmware
{
namespace utils
{
    template<typename ByteStreamT, unsigned long L = 256>
    class Logger
    {
    public:
        template<typename... Args>
        Logger(Args... args)
            : stream_(args...)
        {
        }

        void log(const char* fmt, ...)
        {
            va_list args;

            // message buffer
            char msg[L];

            va_start(args, fmt);
            vsnprintf(msg, L, fmt, args);
            va_end(args);

            const auto len = strlen(msg);

            stream_.write(reinterpret_cast<const uint8_t*>(&msg[0]), len);
        }

    private:
        ByteStreamT stream_;
    };
}
}

#endif
