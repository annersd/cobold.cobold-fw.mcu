#pragma once

#include <string>


class MqttEventArgs
{
private:
    /* data */

public:
    MqttEventArgs(std::string *topic, char *payload);
    ~MqttEventArgs();

    std::string *topic;
    char *payload;
};

MqttEventArgs::MqttEventArgs(std::string *topic, char *payload)
{
    this->topic = topic;
    this->payload = payload;
}

MqttEventArgs::~MqttEventArgs()
{
}
