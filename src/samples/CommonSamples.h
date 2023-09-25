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
      ->registerEventHandler(cobold::sys::EventHandler::create<MqttMessageEventArgs>(
          "cobold.mqtt.message", [](MqttMessageEventArgs *eventArgs) -> void
          {
              // Your event handler code here
              if (eventArgs != nullptr)
              {
                // Check if 'data' is a valid pointer to a std::string
                Serial.println("Received data in event handler");
                Serial.println(eventArgs->payload);
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
    auto component = new cobold::components::DynamicComponent<int>();

    // Bind initialize, update, and configure functions with lambdas that accept DynamicComponent by reference
    component->bind_initialize([](cobold::components::DynamicComponent<int>* componentInstance) -> void
    {
        Serial.println("DynamicComponent initialized");
    });

    component->bind_update([](cobold::components::DynamicComponent<int>* componentInstance) -> void
    {
        Serial.println("DynamicComponent updated");
    });

    component->bind_configure([](cobold::components::DynamicComponent<int>* componentInstance) -> void
    {
        componentInstance->setName("DynamicComponent");
        componentInstance->setId("DynamicComponent");
        Serial.println("DynamicComponent configured");

        componentInstance->bound_object = new cobold::sys::Object<int>(new int(42));
    });


    component->configure();

    componentSvc->addComponent(component);
}

void printBoardInfo(){

  auto logger = cobold::services::getLogger();
  logger->info("Board info: %s", ESP.getChipModel());
  logger->info("Board info: %s", ESP.getSdkVersion());

  logger->info("Free heap: %d", ESP.getFreeHeap());
  logger->info("Free sketch space: %d", ESP.getFreeSketchSpace());
  logger->info("Free PSRAM: %d", ESP.getFreePsram());


}

void addCommonSamples()
{
  printBoardInfo();
  addCommonSample_HostRunning();
  // addCommonSample_ListenToAllMqttMessages();
  addDynamicComponent();
}