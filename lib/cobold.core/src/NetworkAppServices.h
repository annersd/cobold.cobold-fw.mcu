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
            services->addService<cobold::sys::Network>([app](ServiceCollection *services) -> void *
                { 
                    auto wifiSettings = app->getAppConfiguration()
                        ->getSection("cobold.network.wifi");

                    // Serial.println(wifiSettings->getValue("ssid").c_str());
                    // Serial.println(wifiSettings->getValue("password").c_str());

                    return new cobold::sys::Network(app,wifiSettings);
                  
                }); });
    }

    void UseNetworkService(cobold::IApplication *app)
    {
        cobold::SerialLogger *logger = app->getServices()->getService<cobold::SerialLogger>();
        cobold::sys::Network *network = app->getServices()->getService<cobold::sys::Network>();

        // Get the Network service
        logger->info("Setup Network");
        network->setup();
    }
}