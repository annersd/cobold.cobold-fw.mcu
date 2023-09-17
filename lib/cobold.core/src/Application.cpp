
#include "ArduinoLog.h"
#include <AsyncMqttClient.h>

#include "Object.h"
#include "MqttEventArgs.h"

#include "Application.h"
#include "Logger.h"
#include "Network.h"

#include "Scheduler.h"

#include "Configuration.h"
#include "ComponentExtensions.h"

#include "Dispatcher.h"
#include "Event.h"
#include "EventDispatcher.h"




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
                services->addService<cobold::sys::EventDispatcher>([this](ServiceCollection *services) -> void *
                                                      { return new cobold::sys::EventDispatcher(this); });
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

                    return new Network(hb,
                        wifiSettings->getValue("ssid").c_str() , 
                        wifiSettings->getValue("password").c_str() ); });
            });

        // hostBuilder->configureServices(
        //     [](ServiceCollection *services) -> void
        //     {
        //         // Add Network service
        //         services->addService<WebServer>([](ServiceCollection *services) -> void *
        //                                         { return new WebServer(); });
        //     });

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
                    //IPAddress().fromString(mqttConfig->getValue("host").c_str()),
                    IPAddress(192,168,0,67),
                    atoi(mqttConfig->getValue("port").c_str()));

                mqttServer->setClientId(mqttConfig->getValue("clientid").c_str());

                mqttServer->onConnect([hb](bool sessionPresent) {
                    hb->raiseEvent(cobold::sys::Event::create("cobold.mqtt.connected", "void", ""));
                    Serial.println("connected");
                    Serial.print("session present: ");
                    Serial.println(sessionPresent);

                    hb->getServices()->getService<AsyncMqttClient>()->subscribe("cobold/host/ping", 0);
                    hb->getServices()->getService<AsyncMqttClient>()->subscribe("cobold/host/controller/#", 0);
                    hb->getServices()->getService<AsyncMqttClient>()->subscribe("cobold/temperature", 0);
                    hb->getServices()->getService<AsyncMqttClient>()->onMessage([hb](char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) -> void
                    {   

                        hb->raiseEvent(cobold::sys::Event::create("cobold.mqtt.message", "string", 
                        new MqttEventArgs(topic, payload)));

                        if (strcmp(topic, "cobold/host/ping") == 0)
                        {
                            hb->getServices()->getService<AsyncMqttClient>()->publish("cobold/host/pong", 0, false, "pong");
                        }
                        else if (strcmp(topic, "cobold/host/controller/reset") == 0)
                        {
                            ESP.restart();
                        }
                        else if (strcmp(topic, "cobold/host/controller/schedule/message")  == 0)
                        {
                            auto payloadString = new std::string(payload);
                            // hb->getServices()->getService<Scheduler>()->schedule(
                            //     1000, [hb, payloadString](const Scheduler::StateObject &state) -> void
                            //     { 
                            //         Serial.println("Message from MQTT");
                            //         Serial.println(payloadString.c_str());
                            //     }, "MQTTMessage", 10000, Scheduler::StateObject());

                            hb->raiseEvent(cobold::sys::Event::create("cobold.mqtt.message", "string", payloadString));
                        }
                       
                        
                    });
                });

                mqttServer->onDisconnect([hb](AsyncMqttClientDisconnectReason reason) {
                    hb->raiseEvent(cobold::sys::Event::create("cobold.mqtt.disconnected", "void", ""));
                    
                    hb->getServices()->getService<Scheduler>()->schedule(
                        1000, [hb](const Scheduler::StateObject &state) -> void
                        { 
                            Serial.println("Reconnecting to MQTT...");
                            hb->getServices()->getService<AsyncMqttClient>()->connect();
                        },
                        "MQTTReconnect", 10000, Scheduler::StateObject());
                });



                return mqttServer; });
            });
    }

    void Application::setup()
    {
        // Implement your setup logic here

        // hostBuilder->configureServices(
        //     [](ServiceCollection *services) -> void
        //     {
        //         services->addService<AsyncElegantOtaClass>(&AsyncElegantOTA);
        //     });

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
    
        scheduler = getServices()->getService<Scheduler>();
    }

    void Application::loop()
    {
        // Serial.println("Loop");
        // Implement your loop logic here
        scheduler->run();

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

        // start the webserver
        // logger->info("Start Webserver");
        // getServices()->getService<WebServer>()->start();

        // start the mqtt client
        logger->info("Start MQTT");
        auto mqttClient = getServices()->getService<AsyncMqttClient>();

        mqttClient->onConnect([mqttClient](bool sessionPresent) {
        
        mqttClient->setWill("cobold/host/connected", 0, true, "false");
        mqttClient->publish("cobold/host/isconnected", 0, false, "true");
        });
        mqttClient->connect();

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

    void Application::raiseEvent(cobold::sys::Event* event)
    {
        logger->debug("Raise event %s", event->getSource().c_str());
        auto dispatcher = getServices()->getService<cobold::sys::EventDispatcher>();

        dispatcher->dispatch(event);
    }
} // namespace cobold
