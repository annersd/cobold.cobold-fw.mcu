
#include "Cobold.hpp"
#include "CoboldHosting.hpp"
#include "Scheduler.h"
#include "secrets.h"
#include "EventDispatcher.h"

void setupExamples()
{
  // put your setup code here, to run once:

  cobold::app->getServices()->getService<Scheduler>()->scheduleInterval(
      1000, [](const Scheduler::StateObject &state) -> void
      { Serial.println("Hello World"); },
      "HelloWorld", 5000, Scheduler::StateObject());

  // cobold::app->getServices()->getService<Scheduler>()->scheduleInterval(
  //     1000, [](const Scheduler::StateObject &state) -> void
  //     {  cobold::app->raiseEvent("setup", new std::string("Hello World from setup"));},
  //     "", 1000, Scheduler::StateObject());
}


void setup()
{
  // put your setup code here, to run once:

Serial.begin(115200);
  cobold::app = new cobold::Application(new cobold::hosting::HostBuilder());

  cobold::app->getAppConfiguration()->setValue("cobold.network.wifi.ssid", "");
  cobold::app->getAppConfiguration()->setValue("cobold.network.wifi.password", "");
  
  configureApplicationSecrets();

  cobold::app->preSetup();
  cobold::app->setup();

  auto webserver = cobold::app->getServices()->getService<WebServer>();

  webserver->getServer()->on("/api/v1/system", HTTP_GET, [](AsyncWebServerRequest *request)
                             { AsyncJsonResponse *response = new AsyncJsonResponse(false, 4096); });
  // (Optional) Add sketch size and free PSRAM (if available)
  cobold::app->run();

  setupExamples();

  cobold::app->getServices()
    ->getService<EventDispatcher>()
    ->registerEventHandler<std::string>("setup", [](std::string *data) -> void
    { 
        if (data != nullptr) {
            // Check if 'data' is a valid pointer to a std::string
            Serial.println("Hello World from event handler");
            // Serial.println(data->c_str());
        } else {
            // Handle the case when 'data' is null
            Serial.println("Received null data in event handler");
        }
    });

     cobold::app->raiseEvent("setup", new std::string("Hello World from setup"));
 

  }

void loop()
{
  // put your main code here, to run repeatedly:
  cobold::app->loop();

}

