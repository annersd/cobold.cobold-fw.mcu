#pragma once

#include <IApplication.h>
#include <AsyncMqttClient.h>
#include "Logger.h"
#include "IConfiguration.h"
#include "MqttEventArgs.h"
#include <vector>
#include <regex>
#include <string>

namespace cobold::services
{
    namespace mqtt
    {
        typedef std::function<void(MqttEventArgs *eventargs)> OnMessageCallback;
    }; // namespace mqtt

    class Mqtt
    {
    private:
        cobold::configuration::IConfiguration *mqttConfig;
        cobold::IApplication *app;
        std::vector<mqtt::OnMessageCallback> onMessageCallbacks;
        cobold::Logger *logger;

        std::string mqttPrefix = "cobold/";
        std::string mqttNodePrefix = "";

    public:
        Mqtt(cobold::IApplication *app, cobold::configuration::IConfiguration *mqttConfig)
        {
            this->app = app;
            this->mqttConfig = mqttConfig;
            this->logger = app->getServices()->getService<cobold::Logger>();

            mqttNodePrefix = mqttPrefix + mqttConfig->getValue("clientid") + "/";

            Serial.println("Configuring MQTT");
            Serial.println(mqttNodePrefix.c_str());
        }

        void onMessage(mqtt::OnMessageCallback callback)
        {
            this->onMessageCallbacks.push_back(callback);
        }

        void handleMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
        {   
            auto top = std::string(topic);
            top = std::regex_replace(top, std::regex(mqttNodePrefix), "$local/");

            auto eventArgs = new MqttEventArgs(new std::string(top.c_str()), payload);
          
            // for (auto &callback : this->onMessageCallbacks)
            // {
            //     callback(eventArgs);
            // }

            logger->debug("Dispatching local event: %s", top.c_str());
            app->raiseEvent(cobold::sys::Event::create("cobold.mqtt.message", "string", eventArgs));
        }

        std::string getMqttNodePrefix()
        {
            return this->mqttNodePrefix;
        }
    };

}; // namespace cobold::services