#pragma once


class MqttEventArgs
{
private:
    /* data */

public:
    MqttEventArgs(char *topic, char *payload   );
    ~MqttEventArgs();

    char *topic;
    char *payload;
};

MqttEventArgs::MqttEventArgs(char *topic, char *payload)
{
    this->topic = topic;
    this->payload = payload;
}

MqttEventArgs::~MqttEventArgs()
{
}
