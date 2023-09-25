#pragma once

#include "IApplication.h"
#include "LoggerFactory.h"

namespace cobold::services
{

    void AddLogService(cobold::IApplication *app)
    {
        app->getHostBuilder()->configureServices([app](ServiceCollection *services) -> void
                                                 {
            // Add Log service
            services->addService<cobold::LoggerFactory>([app](ServiceCollection *services) -> void *
                { 
                    return new cobold::LoggerFactory(app);
                  
                }); });
    }

    void UseLogService(cobold::IApplication *app)
    {
        cobold::SerialLogger *logger = app->getServices()->getService<cobold::SerialLogger>();
        cobold::LoggerFactory *factory = app->getServices()->getService<cobold::LoggerFactory>();

        // Get the Log service
        logger->info("Setup Log Service");
        factory->onLogMessage.add([logger](LogMessage *event) -> void
                                  {
            // switch by (event->level)
            
            switch (event->level)
            {
            case LOG_LEVEL_FATAL:
                logger->fatal(event->message);
                break;
            case LOG_LEVEL_ERROR:
                logger->error(event->message);
                break;
            case LOG_LEVEL_WARNING:
                logger->warning(event->message);
                break;
            case LOG_LEVEL_INFO:
                logger->info(event->message);
                break;
            case LOG_LEVEL_TRACE:
                logger->debug(event->message);
                break;
            case LOG_LEVEL_VERBOSE:
                logger->verbose(event->message);
                break;
            default:
                break;
            } });
    }
}