#pragma once
#include "ArduinoLog.h"

namespace cobold
{
    class Logger
    {
    public:
        Logger()
        {
        }

        bool isEnabled(int level) const
        {
            return Log.getLevel() >= level;
        }

        template <class T, typename... Args>
        void fatal(T msg, Args... args)
        {
            Log.fatalln(msg, args...);
        }

        template <class T, typename... Args>
        void error(T msg, Args... args)
        {
            Log.errorln(msg, args...);
        }

        template <class T, typename... Args>
        void warning(T msg, Args... args)
        {
            Log.warningln(msg, args...);
        }

        template <class T, typename... Args>
        void info(T msg, Args... args)
        {
            Log.infoln(msg, args...);
        }

        template <class T, typename... Args>
        void debug(T msg, Args... args)
        {
            Log.traceln(msg, args...);
        }

        template <class T, typename... Args>
        void verbose(T msg, Args... args)
        {
            Log.verboseln(msg, args...);
        }
    };
}
