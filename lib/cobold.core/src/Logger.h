#pragma once
#include "ArduinoLog.h"

namespace cobold
{
    class Logger
    {
    private:
        SemaphoreHandle_t mutex;

    public:
        Logger()
        {
            this->mutex = xSemaphoreCreateMutex();
        }

        bool isEnabled(int level) const
        {
            return Log.getLevel() >= level;
        }

        template <class T, typename... Args>
        void fatal(T msg, Args... args)
        {
            xSemaphoreTake(mutex, portMAX_DELAY);
            Log.fatalln(msg, args...);
            xSemaphoreGive(mutex);
        }

        template <class T, typename... Args>
        void error(T msg, Args... args)
        {
            xSemaphoreTake(mutex, portMAX_DELAY);
            Log.errorln(msg, args...);
            xSemaphoreGive(mutex);
        }

        template <class T, typename... Args>
        void warning(T msg, Args... args)
        {
            xSemaphoreTake(mutex, portMAX_DELAY);
            Log.warningln(msg, args...);
            xSemaphoreGive(mutex);
        }

        template <class T, typename... Args>
        void info(T msg, Args... args)
        {
            xSemaphoreTake(mutex, portMAX_DELAY);
            Log.infoln(msg, args...);
            xSemaphoreGive(mutex);
        }

        template <class T, typename... Args>
        void debug(T msg, Args... args)
        {
            xSemaphoreTake(mutex, portMAX_DELAY);
            Log.traceln(msg, args...);
            xSemaphoreGive(mutex);
        }

        template <class T, typename... Args>
        void verbose(T msg, Args... args)
        {
            xSemaphoreTake(mutex, portMAX_DELAY);
            Log.verboseln(msg, args...);
            xSemaphoreGive(mutex);
        }
    };
}
