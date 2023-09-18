#pragma once

#include "Cobold.hpp"

class MqttTempSensorOptions : public cobold::sys::Options
{
private:
    /* data */
    std::string topic;

public:
    MqttTempSensorOptions() : cobold::sys::Options(){};
    MqttTempSensorOptions(cobold::configuration::IConfiguration *configuration) : cobold::sys::Options(configuration){};
    ~MqttTempSensorOptions() = default;

protected:
    void parseConfiguration() override
    {
        topic = configuration->getValue("topic");
    }
};