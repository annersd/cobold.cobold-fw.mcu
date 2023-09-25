#pragma once

#include "Cobold.hpp"
#include "Node.h"

void addCommonSample_HostRunning()
{
  auto mqttClient = cobold::app->getServices()->getService<AsyncMqttClient>();

  // cobold::app->getServices()->getService<Scheduler>()->scheduleInterval(
  //     1000, [mqttClient](const Scheduler::StateObject &state) -> void
  //     { 
  //       // Serial.println("...alive..."); 


  //       std::string msg = "still running. Heap: " + std::to_string(ESP.getFreeHeap());
  //       mqttClient->publish(
  //         ("cobold/" + cobold::app->getServices()->getService<cobold::Node>()->name + "/host/running").c_str()
  //         , 0, false, msg.c_str()); },
  //     "HelloWorld", 10000, Scheduler::StateObject());
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

    auto logger = cobold::app->getServices()->getService<cobold::LoggerFactory>()->getLogger("Dynamic Component");

    // Bind initialize, update, and configure functions with lambdas that accept DynamicComponent by reference
    component->bind_initialize([logger](cobold::components::DynamicComponent<int>* componentInstance) -> void
    {
        logger->info("DynamicComponent initialized");
    });

    component->bind_update([logger](cobold::components::DynamicComponent<int>* componentInstance) -> void
    {
        logger->info("DynamicComponent updated");
    });

    component->bind_configure([logger](cobold::components::DynamicComponent<int>* componentInstance) -> void
    {
        componentInstance->setName("DynamicComponent");
        componentInstance->setId("DynamicComponent");
        logger->info("DynamicComponent configured");

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