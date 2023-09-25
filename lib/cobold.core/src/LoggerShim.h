#pragma once

#include <Arduino.h>
#include <exception>
#include "StreamRenderer.h"
#include "Delegates.h"

namespace cobold
{
    class LogMessage
    {
    public:
        LogMessage(const char *name, int level, const char *message, std::exception *exception = nullptr)
        {
            this->name = name;
            this->level = level;
            this->message = message;
            this->exception = exception;
        }
        ~LogMessage()
        {
            free((void *)this->message);
        }

        const char *name;
        int level;
        const char *message;
        std::exception *exception;
    };

    class Logger
    {
    protected:
        const char *name;
        static cobold::sys::StreamRenderer renderer;

        LogMessage createLogMessage(const char *name, int level, const char *message)
        {
            LogMessage event = LogMessage(name, level, message);
            return event;
        }

    public:
        cobold::sys::ActionDelegate<LogMessage *> onLogMessage;

        Logger(const char *name)
        {
            this->name = name;
        }

        // add all the log methods from Logger class
        template <class T, typename... Args>
        void fatal(T msg, Args... args)
        {
            auto event = new LogMessage(name, LOG_LEVEL_FATAL, renderer.render(msg, args...));
            onLogMessage.invoke(event);
            delete event;
        }

        template <class T, typename... Args>
        void fatal(std::exception ex, T msg, Args... args)
        {
            auto event = new LogMessage(name, LOG_LEVEL_FATAL, renderer.render(msg, args...));
            onLogMessage.invoke(event);
            delete event;
        }

        template <class T, typename... Args>
        void error(T msg, Args... args)
        {
            auto event = new LogMessage(name, LOG_LEVEL_ERROR, renderer.render(msg, args...));
            onLogMessage.invoke(event);
            delete event;
        }

        template <class T, typename... Args>
        void error(std::exception ex, T msg, Args... args)
        {
            auto event = new LogMessage(name, LOG_LEVEL_ERROR, renderer.render(msg, args...));
            onLogMessage.invoke(event);
            delete event;
        }

        template <class T, typename... Args>
        void warning(T msg, Args... args)
        {
            auto event = new LogMessage(name, LOG_LEVEL_WARNING, renderer.render(msg, args...));
            onLogMessage.invoke(event);
            delete event;
        }

        template <class T, typename... Args>
        void info(T msg, Args... args)
        {
            auto event = new LogMessage(name, LOG_LEVEL_INFO, renderer.render(msg, args...));
            onLogMessage.invoke(event);
            delete event;
        }

        template <class T, typename... Args>
        void debug(T msg, Args... args)
        {
            auto event = new LogMessage(name, LOG_LEVEL_TRACE, renderer.render(msg, args...));
            onLogMessage.invoke(event);
            delete event;
        }

        template <class T, typename... Args>
        void verbose(T msg, Args... args)
        {
            auto event = new LogMessage(name, LOG_LEVEL_VERBOSE, renderer.render(msg, args...));
            onLogMessage.invoke(event);
            delete event;
        }
    };

    cobold::sys::StreamRenderer Logger::renderer = cobold::sys::StreamRenderer();

} // namespace cobold