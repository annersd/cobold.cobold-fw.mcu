#pragma once

#include "IApplication.h"
#include "Scheduler.h"

namespace cobold::services
{
    void AddSchedulerService(cobold::IApplication *app)
    {
        app->getHostBuilder()->configureServices([app](ServiceCollection *services) -> void
                                                 {
            // Add  service
            services->addService<Scheduler>([app](ServiceCollection *services) -> void *
                { return new Scheduler(app); });
             });
    }

    void UseSchedulerService(cobold::IApplication *app)
    {
        auto logger = app->getServices()->getService<cobold::Logger>();
        logger->info("Setup Scheduler");

        auto scheduler = app->getServices()->getService<Scheduler>();
        app->onLoop([scheduler]() -> void
                    {
            scheduler->run();
        });
    }
}