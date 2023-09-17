#pragma once

#include "IApplication.h"
#include "Network.h"

namespace cobold::services
{

    void AddNetworkService(cobold::IApplication *app)
    {
        app->getHostBuilder()->configureServices([app](ServiceCollection *services) -> void
                                                 {
            // Add Network service
            services->addService<Network>([app](ServiceCollection *services) -> void *
                { 
                    auto wifiSettings = app->getAppConfiguration()
                        ->getSection("cobold.network.wifi");

                    // Serial.println(wifiSettings->getValue("ssid").c_str());
                    // Serial.println(wifiSettings->getValue("password").c_str());

                    return new Network(app,
                        wifiSettings->getValue("ssid").c_str() , 
                        wifiSettings->getValue("password").c_str() ); 
                }); });
    }

    void UseNetworkService(cobold::IApplication *app)
    {
        cobold::Logger *logger = app->getServices()->getService<cobold::Logger>();
        Network *network = app->getServices()->getService<Network>();

        // Get the Network service
        logger->info("Setup Network");
        network->setup();
    }
}