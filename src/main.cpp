
#include "Cobold.hpp"
#include "CoboldHosting.hpp"
#include "MqttEventArgs.h"

#include "secrets.h"
#include <ArduinoJson.h>



struct Temperature
{
  float value;
};

void sampleTempSensor()
{
  // auto mqttclient = cobold::app->getServices()->getService<AsyncMqttClient>();

  // mqttclient->subscribe("cobold/temperature", 0);
  // mqttclient->onMessage([](char *topic, char *payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) -> void
  //                       {
  //                         if (strcmp(topic, "cobold/temperature") == 0)
  //                         {
  //                           // decode json to Temperature struct
  //                           Temperature *temp = new Temperature();
  //                           temp->value = 0.0f;

  //                           StaticJsonDocument<512> doc;

  //                           // Deserialize the JSON document
  //                           DeserializationError error = deserializeJson(doc, payload);

  //                           // Test if parsing succeeds.
  //                           if (error)
  //                           {
  //                             Serial.print(F("deserializeJson() failed: "));
  //                             Serial.println(error.c_str());
  //                             return;
  //                           }

  //                           // Fetch values.
  //                           temp->value = doc["value"];



  //                           cobold::app->raiseEvent(cobold::sys::Event::create("cobold.temperature", "Temperature", 
  //                           new cobold::sys::Object<Temperature>(temp) ));
  //                         } });

  auto eventDispatcher =  cobold::app->getServices()->getService<cobold::sys::EventDispatcher>();

  eventDispatcher->registerEventHandler(
    cobold::sys::EventHandler::create<MqttEventArgs>("cobold.mqtt.message", "string", [](MqttEventArgs *eventData) -> void
                                                                            {
                                                                              // Your event handler code here
                                                                              if (eventData != nullptr)
                                                                              {
                                                                                // Check if 'data' is a valid pointer to a std::string
                                                                                Serial.println("Received an mqtt message");
                                                                                Serial.println(eventData->payload);
                                                                              }
                                                                            }));

      eventDispatcher->registerEventHandler(cobold::sys::EventHandler::create<cobold::sys::Object<Temperature>>("cobold.temperature", "Temperature", [](cobold::sys::Object<Temperature> *eventData) -> void
                                                                            {
                                                                              // Your event handler code here
                                                                              if (eventData != nullptr)
                                                                              {
                                                                                // Check if 'data' is a valid pointer to a std::string
                                                                                // Serial.println("Received data in event handler");
                                                                                auto value = eventData->get()->value;
                                                                                Serial.println(value);
                                                                              } }));
}

void setupExamples()
{
  // put your setup code here, to run once:

  auto mqttClient = cobold::app->getServices()->getService<AsyncMqttClient>();

  cobold::app->getServices()->getService<Scheduler>()->scheduleInterval(
      1000, [mqttClient](const Scheduler::StateObject &state) -> void
      { 
        Serial.println("...alive..."); 
        
      mqttClient->publish("cobold/host/running", 0, false, "still running"); },
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

  // auto webserver = cobold::app->getServices()->getService<WebServer>();

  // webserver->getServer()->on("/api/v1/system", HTTP_GET, [](AsyncWebServerRequest *request)
  //                            { AsyncJsonResponse *response = new AsyncJsonResponse(false, 4096); });
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

  cobold::app->getServices()
      ->getService<cobold::sys::EventDispatcher>()
      ->registerEventHandler(cobold::sys::EventHandler::create<std::string>("cobold.mqtt.message", "string", [](std::string *eventData) -> void
                                                                            {
                                                                              // Your event handler code here
                                                                              if (eventData != nullptr)
                                                                              {
                                                                                // Check if 'data' is a valid pointer to a std::string
                                                                                // Serial.println("Received data in event handler");
                                                                                Serial.println(eventData->c_str());
                                                                              }
                                                                              else
                                                                              {

                                                                                // Handle the case when 'data' is null
                                                                                // Serial.println("Received null data in event handler");
                                                                              } }));

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
    Serial.println("End of message"); });

    

   
        sampleTempSensor();

       
}

void loop()
{
  vTaskDelay(100 / portTICK_PERIOD_MS);
  // put your main code here, to run repeatedly:
  cobold::app->loop();
}
