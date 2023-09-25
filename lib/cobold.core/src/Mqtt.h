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
        typedef std::function<void(MqttMessageEventArgs *eventargs)> OnMessageCallback;

        struct MessageCallback
        {
            std::string topic;
            OnMessageCallback callback;
        };

        
    }; // namespace mqtt

    class Mqtt
    {
    private:
        cobold::configuration::IConfiguration *mqttConfig;
        cobold::IApplication *app;
        std::vector<mqtt::MessageCallback> onMessageCallbacks;
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

        void onMessage(std::string topic, mqtt::OnMessageCallback callback)
        {
            this->onMessageCallbacks.push_back(mqtt::MessageCallback{topic, callback});
        }

        void handleMqttMessage(char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
        {   
            logger->debug("Dispatching mqtt event: %s", topic);
            for (auto callback : this->onMessageCallbacks)
            {
                if (std::regex_match(std::string(topic), std::regex(callback.topic)))
                {
                    callback.callback(new MqttMessageEventArgs(topic, payload));
                }
            }

            auto top = std::string(topic);
            top = std::regex_replace(top, std::regex(mqttNodePrefix), "$local/");

            auto eventArgs = new MqttMessageEventArgs(top.c_str(), payload);
          
            logger->debug("Dispatching local event: %s", top.c_str());
            app->raiseEvent("cobold.mqtt.message", eventArgs);
        }

        std::string getMqttNodePrefix()
        {
            return this->mqttNodePrefix;
        }
    };

}; // namespace cobold::services