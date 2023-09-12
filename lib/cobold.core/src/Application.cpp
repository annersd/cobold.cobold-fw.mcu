#include "ArduinoLog.h"
#include "Application.h"
#include "Logger.h"
#include "Network.h"
#include "WebServer.h"
#include "Scheduler.h"
#include <AsyncElegantOTA.h>
#include <WebServer.h>
#include "Configuration.h"
#include "ComponentExtensions.h"
#include <AsyncMqttClient.h>
#include "Dispatcher.h"
#include "Event.h"

namespace cobold
{
    Application::Application(cobold::hosting::IHostBuilder *hostBuilder)
    {
        // Initialize Arduino Framweork
        Serial.begin(115200);

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

        hostBuilder->configureServices(
            [this](ServiceCollection *services) -> void
            {
                // Add Dispatcher
                services->addService<dispatch_queue>([this](ServiceCollection *services) -> void *
                                                     { return new dispatch_queue("main"); });
            });

        hostBuilder->configureServices(
            [this](ServiceCollection *services) -> void
            {
                // Add EventDispatcher
                services->addService<EventDispatcher>([this](ServiceCollection *services) -> void *
                                                      { return new EventDispatcher(this); });
            });

        auto hb = this;

        hostBuilder->configureServices(
            [hb](ServiceCollection *services) -> void
            {
                // Add Network service
                services->addService<Network>([hb](ServiceCollection *services) -> void *
                                              { 
                    auto wifiSettings = hb->getAppConfiguration()
                        ->getSection("cobold.network.wifi");

                    // Serial.println(wifiSettings->getValue("ssid").c_str());
                    // Serial.println(wifiSettings->getValue("password").c_str());

                    return new Network(
                        wifiSettings->getValue("ssid").c_str() , 
                        wifiSettings->getValue("password").c_str() ); });
            });

        hostBuilder->configureServices(
            [](ServiceCollection *services) -> void
            {
                // Add Network service
                services->addService<WebServer>([](ServiceCollection *services) -> void *
                                                { return new WebServer(); });
            });

        hostBuilder->configureServices(
            [this](ServiceCollection *services) -> void
            {
                auto app = this;
                // Add Scheduler service
                services->addService<Scheduler>([app](ServiceCollection *services) -> void *
                                                { return new Scheduler(app); });
            });

        hostBuilder->configureServices(
            [hb](ServiceCollection *services) -> void
            {
                // Add MQTT service
                Serial.println("Configuring MQTT");

                services->addService<AsyncMqttClient>([hb](ServiceCollection *services) -> void *
                                                      { 
                    Serial.println("Configuring MQTT"); 
                auto mqttConfig = hb->getAppConfiguration()
                    ->getSection("cobold.mqtt");
                AsyncMqttClient* mqttServer = new AsyncMqttClient();
                mqttServer->setServer(
                    //mqttConfig->getValue("host").c_str(), 
                    IPAddress(192, 168, 0, 67),
                    atoi(mqttConfig->getValue("port").c_str()));
                return mqttServer; });
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

        // Get the service collection
        services = host->getServices();

        // Get the logger
        logger = services->getService<Logger>();

        // Get the Network service
        logger->info("Setup Network");
        auto network = getServices()->getService<Network>();
        network->setup();

        // Assign server to OTA class
        // logger->info("Setup OTA");
        // getServices()->getService<AsyncElegantOtaClass>()->begin(getServices()->getService<WebServer>()->getServer());
    }

    void Application::loop()
    {
        // Implement your loop logic here
        getServices()->getService<Scheduler>()->run();

        auto components = cobold::components::ComponentExtensions::GetComponents(getServices());

        // call update on each component
        for (auto component : components)

        {
            component->update();
        }

        // as long as the scheduler is running from the outer loop, we delay here
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    void Application::run()
    {
        // Implement your run logic here

        // start the webserver
        // logger->info("Start Webserver");
        // getServices()->getService<WebServer>()->start();

        // start the mqtt client
        logger->info("Start MQTT");
        getServices()->getService<AsyncMqttClient>()->connect();

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
        auto dispatcher = getServices()->getService<dispatch_queue>();
        dispatcher->dispatch(function);
    }

    void Application::raiseEvent(std::string eventName, void* eventPayload)
    {
        auto dispatcher = getServices()->getService<EventDispatcher>();
        dispatcher->dispatch(eventName, eventPayload);
    }
}
