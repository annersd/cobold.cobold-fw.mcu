#pragma once

#include "Cobold.hpp"

void addCommonSamples_HostRunning(){
auto mqttClient = cobold::app->getServices()->getService<AsyncMqttClient>();

  cobold::app->getServices()->getService<Scheduler>()->scheduleInterval(
      1000, [mqttClient](const Scheduler::StateObject &state) -> void
      { 
        Serial.println("...alive..."); 
        
      mqttClient->publish("cobold/host/running", 0, false, "still running"); },
      "HelloWorld", 10000, Scheduler::StateObject());
}

void addCommonSamples()
{
    addCommonSamples_HostRunning();



  cobold::app->getServices()
      ->getService<cobold::sys::EventDispatcher>()
      ->registerEventHandler(cobold::sys::EventHandler::create<std::string>("main.setup", "type", [](std::string *eventData) -> void
                                                                            {
    // Your event handler code here
    if (eventData != nullptr) {
        // Check if 'data' is a valid pointer to a std::string
        Serial.println("Received data in event handler");
        Serial.println(eventData->c_str());
    } else {
        // Handle the case when 'data' is null
        Serial.println("Received null data in event handler");
    } }));

  // this makes trouble with the scheduler
  // cobold::app->raiseEvent(cobold::sys::Event::create("main.setup", "type", new std::string("Hello World from setup")));

  cobold::app->getServices()
      ->getService<cobold::sys::EventDispatcher>()
      ->registerEventHandler(cobold::sys::EventHandler::create<std::string>("cobold.mqtt.message", "string", [](std::string *eventData) -> void
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

  auto mqttClient = cobold::app->getServices()->getService<AsyncMqttClient>();
  mqttClient->subscribe("test/lol", 0);
  mqttClient->onMessage([](char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) -> void
                        {
    Serial.println("Received message");
    Serial.println(topic);
    Serial.println(payload);
    Serial.println(len);
    Serial.println(index);
    Serial.println(total);
    Serial.println("End of message"); });
}