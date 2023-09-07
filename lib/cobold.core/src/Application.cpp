#include "ArduinoLog.h"
#include "Application.h"
#include "Logger.h"

namespace cobold
{
    Application::Application(cobold::hosting::IHostBuilder *hostBuilder) 
    {
        // Create an instance of the HostBuilder class
        this->hostBuilder = hostBuilder;

        // Configure the host

        // Configure the app configuration
        hostBuilder->configureServices(
            [](ServiceCollection *services) -> void
            {
                // Add Network service
                // services->addService<Network>([](ServiceCollection *services) -> void *
                //                                { return new Network("", ""); });
            });

        // Build the host
        host = hostBuilder->build();

        // Get the service collection
        services = host->getServices();
    }

    Application::~Application()
    {
        // Clean up resources, if needed
    }

    void Application::preSetup()
    {
        // Implement your preSetup logic here
    }

    void Application::setup()
    {
        // Start the host
        host->start();
    }

    void Application::loop()
    {
        // Implement your loop logic here
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
}
