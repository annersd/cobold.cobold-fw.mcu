#pragma once

#include "Cobold.hpp"
#include "MqttEventArgs.h"

struct Temperature
{
  float value;
};

void createTempEventfromMqttMessage(MqttEventArgs *eventArgs)
{

  Serial.println("createTempEventfromMqttMessage");
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

    cobold::app->raiseEvent(cobold::sys::Event::create("cobold.temperature", "Temperature",
                                                       new cobold::sys::Object<Temperature>(temp)));
  }
}

void addEventSourcedTemperatureSensorSample()
{
  // auto mqttclient = cobold::app->getServices()->getService<AsyncMqttClient>();

  // mqttclient->subscribe("cobold/temperature", 0);
  // mqttclient->onMessage([](char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) -> void
  //                       {
  //                         if (strcmp(topic, "cobold/temperature") == 0)
  //                         {
  //                           // decode json to Temperature struct
  //                           Temperature *temp = new Temperature();
  //                           temp->value = 0.0f;

  //                           StaticJsonDocument<512> doc;

  //                           // Deserialize the JSON document
  //                           DeserializationError error = deserializeJson(doc, payload);

  //                           // Test if parsing succeeds.
  //                           if (error)
  //                           {
  //                             Serial.print(F("deserializeJson() failed: "));
  //                             Serial.println(error.c_str());
  //                             return;
  //                           }

  //                           // Fetch values.
  //                           temp->value = doc["value"];

  //                           cobold::app->raiseEvent(cobold::sys::Event::create("cobold.temperature", "Temperature",
  //                           new cobold::sys::Object<Temperature>(temp) ));
  //                         } });

  auto eventDispatcher = cobold::app->getServices()->getService<cobold::sys::EventDispatcher>();

  eventDispatcher->registerEventHandler(
      cobold::sys::EventHandler::create<MqttEventArgs>("cobold.mqtt.message", "string", [](MqttEventArgs *eventData) -> void
                                                       {
                                                          // Your event handler code here
                                                          if (eventData != nullptr)
                                                          {
                                                            // Check if 'data' is a valid pointer to a std::string
                                                            Serial.println("Received an mqtt message");
                                                            Serial.println(eventData->topic);
                                                            Serial.println(eventData->payload);

                                                            if(strcmp(eventData->topic, "cobold/temperature") == 0)
                                                            {

    // cobold::app->getServices()->getService<Scheduler>()->schedule(
    //   0, [eventData](const Scheduler::StateObject &state) -> void
    //                                                                 {
    //                                                                   Serial.println("Calling event handler");
    //                                                                   try
    //                                                                   {
    //                                                                     createTempEventfromMqttMessage(eventData);
    //                                                                   }
    //                                                                   catch (const std::exception &e)
    //                                                                   {
    //                                                                     Serial.println("Exception in event handler");
    //                                                                   }
    //                                                                 },
    //                                                                 "", 5000, Scheduler::StateObject());
                                                              createTempEventfromMqttMessage(eventData);
                                                            }
                                                          } }));

  eventDispatcher->registerEventHandler(cobold::sys::EventHandler::create<cobold::sys::Object<Temperature>>("cobold.temperature", "Temperature", [](cobold::sys::Object<Temperature> *eventData) -> void
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