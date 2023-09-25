#pragma once

#include <string>
#include <Event.h>


class MqttConnectionEventArgs : public cobold::sys::EventArgs
{
    public:
    MqttConnectionEventArgs(bool connected){
        this->connected = connected;
    }
    ~MqttConnectionEventArgs() = default;

    bool connected;

    const char *getType() const override { return "net.cobold.services.mqtt.connection"; }

    std::string toJson(bool pretty = false) override
    {
        StaticJsonDocument<256> doc;

        doc["connected"] = connected;
        std::string json;
        serializeJson(doc, json);
        return json;
    }

    bool fromJson(std::string json) override
    {
        StaticJsonDocument<256> doc;
        deserializeJson(doc, json);

        bool connected;
        connected = doc["connected"];
        this->connected = connected;

        return true;
    }

};

class MqttMessageEventArgs : public cobold::sys::EventArgs
{
private:
    /* data */

public:
    MqttMessageEventArgs(const char *topic, char *payload){
        this->topic = const_cast<char*>(topic);
        this->payload = payload;
    }
    ~MqttMessageEventArgs() = default;

    char *topic;
    char *payload;

    const char *getType() const override { return "net.cobold.services.mqtt.message"; }

    std::string toJson(bool pretty = false) override
    {
        StaticJsonDocument<256> doc;

        doc["topic"] = topic;
        doc["payload"] = payload;
        std::string json;
        serializeJson(doc, json);
        return json;
    }

    bool fromJson(std::string json) override
    {
        StaticJsonDocument<256> doc;
        deserializeJson(doc, json);

        const char *topic;
        topic = doc["topic"];
        this->topic = const_cast<char*>(topic);

        const char *payload;
        payload = doc["payload"];
        this->payload = const_cast<char*>(payload);

        return true;
    }
};

