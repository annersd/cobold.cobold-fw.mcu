#pragma once

#include "Cobold.hpp"
#include "MqttEventArgs.h"

class TemperatureEventArgs : public cobold::sys::EventArgs
{
public:
  TemperatureEventArgs(float value)
  {
    this->value = value;
  } 

  const char *getType() const override { return "net.cobold.samples.temperature"; }

  float value;

  std::string toJson(bool pretty = false) override
  {
    StaticJsonDocument<256> doc;
    doc["value"] = value;
    std::string json;
    serializeJson(doc, json);
    return json;
  }

  bool fromJson(std::string json) override
  {
    StaticJsonDocument<256> doc;
    deserializeJson(doc, json);
    value = doc["value"];

    return true;
  }
};

void createTempEventfromMqttMessage(MqttMessageEventArgs *eventArgs)
{
  cobold::Logger *logger = cobold::app->getServices()->getService<cobold::Logger>();
  logger->debug("createTempEventfromMqttMessage");
  // if (eventArgs->topic == "cobold/temperature")
  {
    // decode json to Temperature struct
    auto *temp = new TemperatureEventArgs(0.0f);

    StaticJsonDocument<512> doc;

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, eventArgs->payload);

    // Test if parsing succeeds.
    if (error)
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }

    // Fetch values.
    temp->value = doc["value"];

    cobold::app->raiseEvent("sensor1.temperature", temp);
  }
}

void addEventSourcedTemperatureSensorSample()
{

 auto eventDispatcher = cobold::app->getServices()->getService<cobold::sys::EventDispatcher>();
 

  // Register event handler for the event 'cobold.mqtt.message'
  eventDispatcher->registerEventHandler(
      cobold::sys::EventHandler::create<MqttMessageEventArgs>("cobold.mqtt.message", [](MqttMessageEventArgs *eventArgs) -> void
                                                       {
                                                        auto logger = cobold::app->getServices()->getService<cobold::Logger>();

                                                          // Your event handler code here
                                                          if (eventArgs != nullptr)
                                                          {
                                                            if (strcmp(eventArgs->topic, "$local/sensor1/temperature") == 0)
                                                            {

                                                              logger->debug("Received data in event handler");
                                                              logger->debug(eventArgs->topic);
                                                              logger->debug(eventArgs->payload);

                                                              createTempEventfromMqttMessage(eventArgs);
                                                            }
                                                          } }));

  // Register event handler for the event 'cobold.temperature'
  eventDispatcher->registerEventHandler(cobold::sys::EventHandler::create<TemperatureEventArgs>("sensor1.temperature", [](TemperatureEventArgs *eventArgs) -> void
                                                                                                            {
                                                                              // Your event handler code here
                                                                              if (eventArgs != nullptr)
                                                                              {
                                                                                // Check if 'data' is a valid pointer to a std::string
                                                                                // Serial.println("Received data in event handler");
                                                                                auto value = eventArgs->value;
                                                                                Serial.println(value);
                                                                              } }));
}