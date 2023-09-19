
#include "Cobold.hpp"
#include "CoboldHosting.hpp"
#include "CoboldDefaultApp.hpp"

#include "config/mcuConfig.h"

#include "samples/samples.h"

void setup()
{
  // put your setup code here, to run once:

  cobold::app = new cobold::Application(new cobold::hosting::HostBuilder());

  cobold::application::deaultConfigure(cobold::app, deviceConfig, secretsConfig);
  cobold::application::defaultPreSetup(cobold::app);
  cobold::application::defaultSetup(cobold::app);
  
  cobold::app->run();

  setupExamples();
}

void loop()
{
  // put your main code here, to run repeatedly:
  cobold::app->loop();
}
