#pragma once

#include "Cobold.hpp"

void addCommonSample_HostRunning()
{
  auto mqttClient = cobold::app->getServices()->getService<AsyncMqttClient>();

  cobold::app->getServices()->getService<Scheduler>()->scheduleInterval(
      1000, [mqttClient](const Scheduler::StateObject &state) -> void
      { 
        Serial.println("...alive..."); 
        
      mqttClient->publish("cobold/host/running", 0, false, "still running"); },
      "HelloWorld", 10000, Scheduler::StateObject());
}

void addCommonSample_ListenToAllMqttMessages()
{
  cobold::app->getServices()
      ->getService<cobold::sys::EventDispatcher>()
      ->registerEventHandler(cobold::sys::EventHandler::create<std::string>(
          "cobold.mqtt.message", "string", [](std::string *eventData) -> void
          {
              // Your event handler code here
              if (eventData != nullptr)
              {
                // Check if 'data' is a valid pointer to a std::string
                Serial.println("Received data in event handler");
                Serial.println(eventData->c_str());
              }
              else
              {

                // Handle the case when 'data' is null
                // Serial.println("Received null data in event handler");
              } }));
}

void addCommonSamples()
{
  addCommonSample_HostRunning();
  // addCommonSample_ListenToAllMqttMessages();
}