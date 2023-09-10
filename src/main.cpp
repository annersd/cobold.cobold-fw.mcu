
#include "Cobold.hpp"
#include "CoboldHosting.hpp"
#include "Scheduler.h"
#include "secrets.h"

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

}

void loop()
{
  // put your main code here, to run repeatedly:
  cobold::app->loop();

}

void setupExamples()
{
  // put your setup code here, to run once:

  cobold::app->getServices()->getService<Scheduler>()->scheduleInterval(
      1000, [](const Scheduler::StateObject &state) -> void
      { Serial.println("Hello World"); },
      "HelloWorld", 1000, Scheduler::StateObject());
}
