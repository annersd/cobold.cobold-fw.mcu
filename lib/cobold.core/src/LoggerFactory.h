#pragma once

#include <Arduino.h>
#include "Logger.h"
#include "Delegates.h"
#include "LoggerShim.h"
#include "IApplication.h"

namespace cobold
{

    class LoggerFactory
    {
        private:

        cobold::IApplication *app;
        public:

        LoggerFactory(cobold::IApplication *app)
        {
            this->app = app;
        }
        cobold::sys::ActionDelegate< LogMessage *> onLogMessage;
    
        Logger *getLogger(const char *name)
        {
            auto logger = new Logger(name);

            logger->onLogMessage.add([this](LogMessage *event) -> void {
                this->onLogMessage.invoke(event);
            });
            return logger;
        }
    };

} // namespace cobold