#pragma once

#include "Cobold.hpp"
#include "MqttEventArgs.h"

struct Temperature
{
  float value;
};

void createTempEventfromMqttMessage(MqttEventArgs *eventArgs)
{
  cobold::Logger *logger = cobold::app->getServices()->getService<cobold::Logger>();
  logger->debug("createTempEventfromMqttMessage");
  // if (eventArgs->topic == "cobold/temperature")
  {
    // decode json to Temperature struct
    Temperature *temp = new Temperature();
    temp->value = 0.0f;

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

    cobold::app->raiseEvent(cobold::sys::Event::create("sensor1.temperature", "Temperature",
                                                       new cobold::sys::Object<Temperature>(temp)));
  }
}

void addEventSourcedTemperatureSensorSample()
{

 auto eventDispatcher = cobold::app->getServices()->getService<cobold::sys::EventDispatcher>();
 

  // Register event handler for the event 'cobold.mqtt.message'
  eventDispatcher->registerEventHandler(
      cobold::sys::EventHandler::create<MqttEventArgs>("cobold.mqtt.message", "string", [](MqttEventArgs *eventData) -> void
                                                       {
                                                        auto logger = cobold::app->getServices()->getService<cobold::Logger>();

                                                          // Your event handler code here
                                                          if (eventData != nullptr)
                                                          {
                                                            if (strcmp(eventData->topic->c_str(), "$local/sensor1/temperature") == 0)
                                                            {

                                                              logger->debug("Received data in event handler");
                                                              logger->debug(eventData->topic->c_str());
                                                              logger->debug(eventData->payload);

                                                              createTempEventfromMqttMessage(eventData);
                                                            }
                                                          } }));

  // Register event handler for the event 'cobold.temperature'
  eventDispatcher->registerEventHandler(cobold::sys::EventHandler::create<cobold::sys::Object<Temperature>>("sensor1.temperature", "Temperature", [](cobold::sys::Object<Temperature> *eventData) -> void
                                                                                                            {
                                                                              // Your event handler code here
                                                                              if (eventData != nullptr)
                                                                              {
                                                                                // Check if 'data' is a valid pointer to a std::string
                                                                                // Serial.println("Received data in event handler");
                                                                                auto value = eventData->get()->value;
                                                                                Serial.println(value);
                                                                              } }));
}