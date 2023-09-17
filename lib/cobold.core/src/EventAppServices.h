#pragma once

#include "IApplication.h"
#include "EventDispatcher.h"

namespace cobold::services
{
    void AddEventService(cobold::IApplication *app)
    {
        app->getHostBuilder()->configureServices([app](ServiceCollection *services) -> void
                                                 {
            // Add  service
            services->addService<cobold::sys::EventDispatcher>([app](ServiceCollection *services) -> void *
                { return new cobold::sys::EventDispatcher(app); });
             });
    }

    void UseEventService(cobold::IApplication *app)
    {
        auto logger = app->getServices()->getService<cobold::Logger>();
        logger->info("Setup EventDispatcher");

        auto eventDispatcher = app->getServices()->getService<cobold::sys::EventDispatcher>();
        app->onEvent([eventDispatcher, logger](cobold::sys::Event *event) -> void
                     { 
                        logger->verbose("raiseEvent: %s", event->getSource().c_str());
                        eventDispatcher->dispatch(event); 
                        });
    }
}