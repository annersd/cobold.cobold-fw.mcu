#pragma once

#include <IApplication.h>
#include "LoggerShim.h"
#include "AsyncMqttClient.h"
#include "string"
#include "Node.h"

class MqttLogger
{
private:
    cobold::IApplication *app;
    AsyncMqttClient *mqttClient;
    std::string name;


public:
    MqttLogger(cobold::IApplication *app, AsyncMqttClient *mqttClient)
    {
        this->app = app;
        this->mqttClient = mqttClient;

        name = app->getServices()->getService<cobold::Node>()->name;
    }
    ~MqttLogger() = default;

    void handleLogMessage(cobold::LogMessage *event)
    {
        // switch by (event->level)
            switch (event->level)
            {
            case LOG_LEVEL_FATAL:
                mqttClient->publish(("cobold/" + name + "/host/monitor/log/" + std::string(event->name) + "/fatal").c_str(), 0, false, event->message);
                break;
            case LOG_LEVEL_ERROR:
                mqttClient->publish(("cobold/" + name + "/host/monitor/log/" + std::string(event->name) + "/error").c_str(), 0, false, event->message);
                break;
            case LOG_LEVEL_WARNING:
                mqttClient->publish(("cobold/" + name + "/host/monitor/log/" + std::string(event->name) + "/warning").c_str(), 0, false, event->message);
                break;
            case LOG_LEVEL_INFO:
                mqttClient->publish(("cobold/" + name + "/host/monitor/log/" + std::string(event->name) + "/info").c_str(), 0, false, event->message);
                break;
            case LOG_LEVEL_TRACE:
                mqttClient->publish(("cobold/" + name + "/host/monitor/log/" + std::string(event->name) + "/trace").c_str(), 0, false, event->message);
                break;
            case LOG_LEVEL_VERBOSE:
                mqttClient->publish(("cobold/" + name + "/host/monitor/log/" + std::string(event->name) + "/verbose").c_str(), 0, false, event->message);
                break;
            default:
                break;
            }
    }
};