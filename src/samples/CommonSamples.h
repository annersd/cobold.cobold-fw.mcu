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


void addDynamicComponent()
{
    auto componentSvc = cobold::app->getServices()->getService<cobold::components::ComponentSvc>();
    auto component = new cobold::components::DynamicComponent();

    // Bind initialize, update, and configure functions with lambdas that accept DynamicComponent by reference
    component->bind_initialize([](cobold::components::DynamicComponent* componentInstance) -> void
    {
        Serial.println("DynamicComponent initialized");
    });

    component->bind_update([](cobold::components::DynamicComponent* componentInstance) -> void
    {
        Serial.println("DynamicComponent updated");
    });

    component->bind_configure([](cobold::components::DynamicComponent* componentInstance) -> void
    {
        componentInstance->setName("DynamicComponent");
        componentInstance->setId("DynamicComponent");
        Serial.println("DynamicComponent configured");
    });


    component->configure();

    componentSvc->addComponent(component);
}


void addCommonSamples()
{
  addCommonSample_HostRunning();
  // addCommonSample_ListenToAllMqttMessages();
  addDynamicComponent();
}