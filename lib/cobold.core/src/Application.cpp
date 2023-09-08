#include "ArduinoLog.h"
#include "Application.h"
#include "Logger.h"
#include "Network.h"
#include "WebServer.h"
#include "Scheduler.h"
#include <AsyncElegantOTA.h>
#include <WebServer.h>
#include "Configuration.h"

namespace cobold
{
    Application::Application(cobold::hosting::IHostBuilder *hostBuilder)
    {
        // Create an instance of the HostBuilder class
        this->hostBuilder = hostBuilder;

        this->configuration = new cobold::configuration::Configuration();
    }

    Application::~Application()
    {
        // Clean up resources, if needed
    }

    void Application::preSetup()
    {
        // Implement your preSetup logic here
        auto wifiConfig = getAppConfiguration()->getSection("cobold.network.wifi");


        // Configure the host


        // Configure the app configuration
        hostBuilder->configureServices(
            [](ServiceCollection *services) -> void
            {
                // Add Network service
                services->addService<Network>([](ServiceCollection *services) -> void *
                                               { return new Network("", ""); });
            });

        hostBuilder->configureServices(
            [](ServiceCollection *services) -> void
            {
                // Add Network service
                services->addService<WebServer>([](ServiceCollection *services) -> void *
                                               { return new WebServer(); });
            });

        
    }

    void Application::setup()
    {
        // Implement your setup logic here


        hostBuilder->configureServices(
            [](ServiceCollection *services) -> void
            {
                services->addService<AsyncElegantOtaClass>(&AsyncElegantOTA);
            });


        // Build the host
        host = hostBuilder->build();

          // Assign server to OTA class
getServices()->getService<AsyncElegantOtaClass>()
    ->begin(getServices()->getService<WebServer>()->getServer());

        // Get the service collection
        services = host->getServices();
    }

    void Application::loop()
    {
        // Implement your loop logic here
        getServices()->getService<Scheduler>()->run();
    }

    void Application::run()
    {
        // Implement your run logic here
        host->start();
    }

    ServiceCollection *Application::getServices()
    {
        return services;
    }

    cobold::hosting::IHost *Application::getHost()
    {
        return host;
    }

    cobold::hosting::IHostBuilder *Application::getHostBuilder()
    {
        return hostBuilder;
    }

    cobold::configuration::IConfiguration *Application::getAppConfiguration()
    {
        return configuration;
    }
}
