#pragma once

#include "IApplication.h"
#include "ComponentSvc.h"

namespace cobold::services
{
    void AddComponentService(cobold::IApplication *app)
    {
        app->getHostBuilder()->configureServices([app](ServiceCollection *services) -> void
                                                 {
            // Add  service
            services->addService<cobold::components::ComponentSvc>([app](ServiceCollection *services) -> void *
                { return new cobold::components::ComponentSvc(app); }); });
    }

    void UseComponentService(cobold::IApplication *app)
    {
        auto logger = app->getServices()->getService<cobold::Logger>();
        logger->info("Setup Compoent Service");

        auto componentSvc = app->getServices()->getService<cobold::components::ComponentSvc>();
        componentSvc->setup();

        app->onLoop([componentSvc]() -> void
                    { componentSvc->update(); });
    }
}