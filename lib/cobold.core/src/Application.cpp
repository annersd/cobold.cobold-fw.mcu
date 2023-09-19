

#include "ArduinoLog.h"
#include <AsyncMqttClient.h>

#include "Object.h"
#include "Logger.h"
#include "Application.h"
#include "Configuration.h"

namespace cobold
{
    Application::Application(cobold::hosting::IHostBuilder *hostBuilder)
    {
        // Initialize Arduino Framweork
        Serial.begin(115200);
        Serial.println();
        Serial.println("--- Create Application ---");

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

        // Configure the host

        // Configure the app configuration
        hostBuilder->configureServices(
            [this](ServiceCollection *services) -> void
            {
                // Add Application
                services->addService<IApplication>(this);
            });

         
    }

    void Application::setup()
    {
        // Implement your setup logic here

        // Build the host
        host = hostBuilder->build();

        // Get the service collection
        services = host->getServices();

        // Get the logger
        logger = services->getService<Logger>();

    }

    void Application::loop()
    {
        // Serial.println("Loop");
        // Implement your loop logic here
        
        // call the loop callbacks
        for (auto callback : loopCallbacks)
        {
            callback();
        }

        // auto components = cobold::components::ComponentExtensions::GetComponents(getServices());

        // // call update on each component
        // for (auto component : components)

        // {
        //     component->update();
        // }

        // as long as the scheduler is running from the outer loop, we delay here
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    void Application::run()
    {
        // Implement your run logic here

        logger->info("Starting host");
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

    void Application::dispatch(std::function<void()> function)
    {
        for (auto callback : dispatchCallbacks)
        {
            callback(function);
        }
    }

   
    void Application::onLoop(application::OnLoopCallback callback)
    {
        loopCallbacks.push_back(callback);
    }

    void Application::onEvent(std::function<void(cobold::sys::Event*)> callback)
    {
        eventCallbacks.push_back(callback);
    }

    void Application::onDispatch(application::OnDispatchCallback callback)
    {
        dispatchCallbacks.push_back(callback);
    }

    void Application::raiseEvent(cobold::sys::Event* event)
    {
        for (auto callback : eventCallbacks)
        {
            callback(event);
        }
    }


} // namespace cobold
