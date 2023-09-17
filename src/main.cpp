
#include "Cobold.hpp"

#include "CoboldHosting.hpp"

#include "MqttAppServices.h"
#include "NetworkAppServices.h"
#include "SchedulerAppServices.h"
#include "EventAppServices.h"
#include "TaskAppServices.h"

#include "config/mcuConfig.h"

#include "samples/samples.h"

void setup()
{
  // put your setup code here, to run once:

  cobold::app = new cobold::Application(new cobold::hosting::HostBuilder());

  // configure the application
  cobold::app->getAppConfiguration()->update(
      cobold::configuration::Configuration::fromJson(deviceConfig));
  cobold::app->getAppConfiguration()->update(
      cobold::configuration::Configuration::fromJson(secretsConfig));

  cobold::app->preSetup();

  cobold::services::AddTaskService(cobold::app);
  cobold::services::AddEventService(cobold::app);
  cobold::services::AddSchedulerService(cobold::app);

  cobold::services::AddNetworkService(cobold::app);
  cobold::services::AddMqttClientService(cobold::app);

  cobold::app->setup();

  cobold::services::UseTaskService(cobold::app);
  cobold::services::UseEventService(cobold::app);
  cobold::services::UseSchedulerService(cobold::app);

  cobold::services::UseNetworkService(cobold::app);
  cobold::services::UseMqttClientService(cobold::app);

  cobold::app->run();

  setupExamples();
}

void loop()
{
  // put your main code here, to run repeatedly:
  // Serial.println("main::loop");
  cobold::app->loop();
}
