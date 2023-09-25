#pragma once

#include <Arduino.h>
#include <exception>
#include "Logger.h"
#include "StreamRenderer.h"

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

        const char *name;
        int level;
        const char *message;
        std::exception *exception;
    };

    class LoggerShim
    {
    protected:
        Logger *logger;
        const char *name;

        static cobold::sys::StreamRenderer renderer;

        LogMessage createLogMessage(const char *name, int level, const char *message)
        {
            LogMessage event = LogMessage(name, level, message);
            return event;
        }

        void publishLogMessage(LogMessage *event)
        {
            Serial.println(event->message);
        }

    public:
        LoggerShim(Logger *logger, const char *name)
        {
            this->logger = logger;
            this->name = name;
        }

        // add all the log methods from Logger class
        template <class T, typename... Args>
        void fatal(T msg, Args... args)
        {
            auto event = new LogMessage(name, LOG_LEVEL_FATAL, renderer.render(msg, args...));
            publishLogMessage(event);
        }

        template <class T, typename... Args>
        void fatal(std::exception ex, T msg, Args... args)
        {
            auto event = new LogMessage(name, LOG_LEVEL_FATAL, renderer.render(msg, args...));
            publishLogMessage(event);
        }

        template <class T, typename... Args>
        void error(T msg, Args... args)
        {
            auto event = new LogMessage(name, LOG_LEVEL_ERROR, renderer.render(msg, args...));
            publishLogMessage(event);
        }

        template <class T, typename... Args>
        void error(std::exception ex, T msg, Args... args)
        {
            auto event = new LogMessage(name, LOG_LEVEL_ERROR, renderer.render(msg, args...));
            publishLogMessage(event);
        }

        template <class T, typename... Args>
        void warning(T msg, Args... args)
        {
            auto event = new LogMessage(name, LOG_LEVEL_WARNING, renderer.render(msg, args...));
            publishLogMessage(event);
        }

        template <class T, typename... Args>
        void info(T msg, Args... args)
        {
            auto event = new LogMessage(name, LOG_LEVEL_INFO, renderer.render(msg, args...));
            publishLogMessage(event);
        }

        template <class T, typename... Args>
        void debug(T msg, Args... args)
        {
            auto event = new LogMessage(name, LOG_LEVEL_TRACE, renderer.render(msg, args...));
            publishLogMessage(event);
        }

        template <class T, typename... Args>
        void verbose(T msg, Args... args)
        {
            auto event = new LogMessage(name, LOG_LEVEL_VERBOSE, renderer.render(msg, args...));
            publishLogMessage(event);
        }
    };

    cobold::sys::StreamRenderer LoggerShim::renderer = cobold::sys::StreamRenderer();
    
} // namespace cobold