
#include "Cobold.hpp"
#include "CoboldHosting.hpp"

void setup() {
  // put your setup code here, to run once:

  cobold::app = new cobold::Application(new cobold::hosting::HostBuilder());
  cobold::app->preSetup();
  cobold::app->setup();

  auto webserver = cobold::app->getServices()->getService<WebServer>();
  
  webserver->getServer()->on("/api/v1/system", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncJsonResponse *response = new AsyncJsonResponse(false, 4096);
    
});
            // (Optional) Add sketch size and free PSRAM (if available)
  cobold::app->run();

}

void loop() {
  // put your main code here, to run repeatedly:
  cobold::app->loop();
}

