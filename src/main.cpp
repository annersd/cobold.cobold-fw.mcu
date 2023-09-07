
#include "Cobold.hpp"
#include "CoboldHosting.hpp"

void setup() {
  // put your setup code here, to run once:

  cobold::app = new cobold::Application(new cobold::hosting::HostBuilder());
  cobold::app->preSetup();
  cobold::app->setup();
}

void loop() {
  // put your main code here, to run repeatedly:
  cobold::app->loop();
}

