#pragma once

#include "IApplication.h"
#include "Node.h"

namespace cobold::services
{

    void AddNodeService(cobold::IApplication *app)
    {
        app->getHostBuilder()->configureServices([app](ServiceCollection *services) -> void
                                                 {
            // Add Network service
            services->addService<cobold::Node>([app](ServiceCollection *services) -> void *
                { 
                    auto settings = app->getAppConfiguration()
                        ->getSection("cobold.node");

                    return new cobold::Node(app,settings);
                  
                }); });
    }

    void UseNodeService(cobold::IApplication *app)
    {
        cobold::SerialLogger *logger = app->getServices()->getService<cobold::SerialLogger>();
        cobold::Node *node = app->getServices()->getService<cobold::Node>();

        // Get the Network service
        logger->info("Setup Network");
        node->setup();
    }
}