//
// logger.hpp
//
// @author Natesh Narain <nnaraindev@gmail.com>
// @date Oct 31 2020
//

#ifndef FTL_LOGGING_LOGGER_HPP
#define FTL_LOGGING_LOGGER_HPP

#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#define LOG_DEBUG(msg, ...) ftl::logging::SystemLogger::instance().getLogger()->log("[DEBUG] " msg "\n\r", ##__VA_ARGS__)
#define LOG_INFO(msg, ...)  ftl::logging::SystemLogger::instance().getLogger()->log("[INFO ] " msg "\n\r", ##__VA_ARGS__)
#define LOG_WARN(msg, ...)  ftl::logging::SystemLogger::instance().getLogger()->log("[WARN ] " msg "\n\r", ##__VA_ARGS__)
#define LOG_ERROR(msg, ...) ftl::logging::SystemLogger::instance().getLogger()->log("[ERROR] " msg "\n\r", ##__VA_ARGS__)


namespace ftl
{
namespace logging
{
    class LoggerBase
    {
    public:
        virtual void log(const char* fmt, ...) = 0;
    };

    template<typename LoggerOutputT, unsigned long L = 256>
    class Logger : public LoggerBase
    {
    public:
        template<typename... Args>
        Logger(Args... args)
            : output_(args...)
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

            // const auto len = strlen(msg);

            output_.write(reinterpret_cast<const char*>(&msg[0]));
        }

        LoggerOutputT& getOutput()
        {
            return output_;
        }

    private:
        LoggerOutputT output_;
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
