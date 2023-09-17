#pragma once

#include <IApplication.h>
#include "Scheduler.h"
#include "Event.h"
#include "MqttEventArgs.h"
#include <AsyncMqttClient.h>
#include "Mqtt.h"
#include <string>

namespace cobold::services
{

    void AddMqttClientService(cobold::IApplication *app)
    {
        
        app->getHostBuilder()->configureServices([app](ServiceCollection *services) -> void
                                                 {
            //                                         auto logger = app->getServices()->getService<cobold::Logger>();

            // logger->info("Setup MQTT Service");

            services->addService<Mqtt>([app](ServiceCollection *services) -> void *
                { 
                    auto mqttConfig = services->getService<cobold::IApplication>()
                        ->getAppConfiguration()
                        ->getSection("cobold.mqtt");

                    return new Mqtt(app, mqttConfig);
                }); });

        app->getHostBuilder()->configureServices([app](ServiceCollection *services) -> void
                                                 {
            //                                         auto logger = app->getServices()->getService<cobold::Logger>();

            // logger->info("Setup MQTT Client Service");

            services->addService<AsyncMqttClient>([app](ServiceCollection *services) -> void *
                { 
                    Serial.println("Configuring MQTT"); 
                auto mqttConfig = app->getAppConfiguration()
                    ->getSection("cobold.mqtt");
                AsyncMqttClient* mqttServer = new AsyncMqttClient();
                mqttServer->setServer(
                    //IPAddress().fromString(mqttConfig->getValue("host").c_str()),
                    IPAddress(192,168,0,69),
                    atoi(mqttConfig->getValue("port").c_str()));

                mqttServer->setClientId(mqttConfig->getValue("clientid").c_str());

                mqttServer->onConnect([app, mqttConfig](bool sessionPresent) {
                    app->raiseEvent(cobold::sys::Event::create("cobold.mqtt.connected", "void", ""));
                    Serial.println("connected");
                    Serial.print("session present: ");
                    Serial.println(sessionPresent);

                    auto mqttClient = app->getServices()->getService<AsyncMqttClient>();
                    mqttClient->publish("cobold/host/connected", 0, true, "true");
                    mqttClient->subscribe("cobold/host/ping", 0);
                    mqttClient->subscribe("cobold/host/controller/#", 0);

                    // subscribe the whole node
                    mqttClient->subscribe(
                        (new std::string())->append("cobold/")
                            .append(mqttConfig->getValue("clientid")).append("/#").c_str() , 0);

                    mqttClient->subscribe("cobold/host/ping", 0);
                    mqttClient->subscribe("cobold/host/controller/#", 0);
       
                    mqttClient->onMessage([app](char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) -> void
                    {   

                        auto mqttService = app->getServices()->getService<Mqtt>();
                        if (std::string(topic).rfind(mqttService->getMqttNodePrefix(), 0) == 0)
                        {
                            Serial.println("Node Message found");
                            mqttService->handleMqttMessage(topic, payload, properties, len, index, total);
                        }                        

                        app->raiseEvent(cobold::sys::Event::create("cobold.mqtt.message", "string", 
                        new MqttEventArgs(new std::string(topic), payload)));

                        if (strcmp(topic, "cobold/host/ping") == 0)
                        {
                            app->getServices()->getService<AsyncMqttClient>()->publish("cobold/host/pong", 0, false, "pong");
                        }
                        else if (strcmp(topic, "cobold/host/controller/reset") == 0)
                        {
                            ESP.restart();
                        }
                        else if (strcmp(topic, "cobold/host/controller/schedule/message")  == 0)
                        {
                            auto payloadString = new std::string(payload);
                            app->raiseEvent(cobold::sys::Event::create("cobold.mqtt.message", "string", payloadString));
                        }
                       
                        
                    });
                });

                mqttServer->onDisconnect([app](AsyncMqttClientDisconnectReason reason) {
                    app->raiseEvent(cobold::sys::Event::create("cobold.mqtt.disconnected", "void", ""));
                    
                    app->getServices()->getService<Scheduler>()->schedule(
                        4000, [app](const Scheduler::StateObject &state) -> void
                        { 
                            auto mqttClient = app->getServices()->getService<AsyncMqttClient>();
                            if(mqttClient->connected())
                                Serial.println("MQTT is connected");
                            else
                            {
                                Serial.println("Reconnecting to MQTT...");
                                app->getServices()->getService<AsyncMqttClient>()->connect();
                            }
                        },
                        "MQTTReconnect", 10000, Scheduler::StateObject());
                });



                return mqttServer; }); });
    }

    void UseMqttClientService(cobold::IApplication *app)
    {
        auto logger = app->getServices()->getService<cobold::Logger>();
        logger->info("Setup MQTT Client Service");

        auto mqttClient = app->getServices()->getService<AsyncMqttClient>();

        mqttClient->onConnect([mqttClient](bool sessionPresent)
            {
                mqttClient->setWill("cobold/host/connected", 0, true, "false");
                mqttClient->publish("cobold/host/isconnected", 0, false, "true"); 
            });

        mqttClient->connect();
    }
}