
#include "Cobold.hpp"
#include "CoboldHosting.hpp"

#include "secrets.h"


void setupExamples()
{
  // put your setup code here, to run once:

  cobold::app->getServices()->getService<Scheduler>()->scheduleInterval(
      1000, [](const Scheduler::StateObject &state) -> void
      { Serial.println("Hello World"); 
      auto mqttClient = cobold::app->getServices()->getService<AsyncMqttClient>();
      mqttClient->publish("cobold/host/running", 0, false, "still running");
      },
      "HelloWorld", 10000, Scheduler::StateObject());

  // cobold::app->getServices()->getService<Scheduler>()->scheduleInterval(
  //     1000, [](const Scheduler::StateObject &state) -> void
  //     {  cobold::app->raiseEvent("setup", new std::string("Hello World from setup"));},
  //     "", 1000, Scheduler::StateObject());
}

template <typename TEventData>
void registerEventHandler(std::string source, std::function<void(TEventData *)> handler){};

void setup()
{
  Serial.begin(115200);
  Serial.println();

  cobold::sys::Object<std::string> *obj = new cobold::sys::Object<std::string>(new std::string("Hello World"));
  cobold::sys::Object<std::string> obj2(new std::string("Hello World"));

  auto item = cobold::sys::unwrap<std::string>(obj);
  if (item != nullptr)
  {
    Serial.println(item->c_str());
  }

  // Serial.println(obj2.get());

  // put your setup code here, to run once:

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
    Serial.println("End of message");
  });
}

void loop()
{
  vTaskDelay(100 / portTICK_PERIOD_MS);
  // put your main code here, to run repeatedly:
  cobold::app->loop();
}
