//
// logger.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Oct 31 2020
//

#ifndef FTL_UTILS_LOGGER_HPP
#define FTL_UTILS_LOGGER_HPP

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#define LOG_DEBUG(msg, ...) ftl::utils::SystemLogger::instance().getLogger()->log("[DEBUG] " msg "\n\r", ##__VA_ARGS__)
#define LOG_INFO(msg, ...)  ftl::utils::SystemLogger::instance().getLogger()->log("[INFO ] " msg "\n\r", ##__VA_ARGS__)
#define LOG_WARN(msg, ...)  ftl::utils::SystemLogger::instance().getLogger()->log("[WARN ] " msg "\n\r", ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) ftl::utils::SystemLogger::instance().getLogger()->log("[ERROR] " msg "\n\r", ##__VA_ARGS__)


namespace ftl
{
namespace utils
{
    class LoggerBase
    {
    public:
        virtual void log(const char* fmt, ...) = 0;
    };

    template<typename ByteStreamT, unsigned long L = 256>
    class Logger : public LoggerBase
    {
    public:
        template<typename... Args>
        Logger(Args... args)
            : stream_(args...)
        {
        }

        void log(const char* fmt, ...) override
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

    // TODO remove?
    class NoopLogger : public LoggerBase
    {
    public:
        void log(const char* fmt, ...) override
        {
        }

        static NoopLogger& instance()
        {
            static NoopLogger logger;
            return logger;
        }
    };

    class SystemLogger
    {
    public:
        SystemLogger()
            : logger_{&NoopLogger::instance()}
        {
        }

        void setLogger(LoggerBase* const logger)
        {
            if (logger != nullptr)
            {
                logger_ = logger;
            }
        }

        LoggerBase* getLogger()
        {
            return logger_;
        }

        template<typename LoggerT, typename... Args>
        static void initialize(Args... args)
        {
            // SystemLogger::instance().setLogger(new LoggerT{args...});
        }

        static SystemLogger& instance()
        {
            static SystemLogger logger;
            return logger;
        }

    private:
        LoggerBase* logger_;
    };
}
}

#endif
