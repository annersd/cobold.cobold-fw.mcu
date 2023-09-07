#pragma once

#include <Arduino.h>

#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include "AsyncJson.h"
#include "ArduinoJson.h"

class WebServer
{
private:
    AsyncWebServer *server;

public:
    WebServer()
    {
        server = new AsyncWebServer(80);

        server->on("/health", HTTP_GET, [](AsyncWebServerRequest *request)
                   { request->send(200, "text/plain", "OK"); });

        server->on("/status", [](AsyncWebServerRequest *request)
                   {
    // Create a JSON response
    AsyncJsonResponse *response = new AsyncJsonResponse(false, 4096);

    // Add common HTTP headers
    response->addHeader("Server", "ESP Async Web Server");

    // Access the JSON root
    JsonVariant &root = response->getRoot();

    root["timestamp"] = 

    // Add ESP8266/ESP32 system information
    root["heap"] = ESP.getFreeHeap();
    root["free_sketch_space"] = ESP.getFreeSketchSpace();
    root["sketch_md5"] = ESP.getSketchMD5();
    root["flash_chip_size"] = ESP.getFlashChipSize();
    root["flash_chip_speed"] = ESP.getFlashChipSpeed();
    root["cycle_count"] = ESP.getCycleCount();
    root["sdk_version"] = ESP.getSdkVersion();
    root["cpu_freq"] = ESP.getCpuFreqMHz();

    // Add Wi-Fi information
    root["ssid"] = WiFi.SSID();
    root["bssid"] = WiFi.BSSIDstr();
    root["rssi"] = WiFi.RSSI();
    root["local_ip"] = WiFi.localIP().toString();
    // root["gateway_ip"] = WiFi.gatewayIP().toString();
    // root["subnet_mask"] = WiFi.subnetMask().toString();
    root["mac_address"] = WiFi.macAddress();

            // (Optional) Add sketch size and free PSRAM (if available)
#ifdef ESP32
    root["sketch_size"] = ESP.getSketchSize();
    root["free_psram"] = ESP.getFreePsram();
#endif

    // Set the response content length
    response->setLength();

    // Send the JSON response
    request->send(response); });
    };

    ~WebServer();
    void setup(){};
    void loop(){};

    void start()
    {
        server->begin();
    };

    void stop()
    {
        server->end();
    };

    AsyncWebServer *getServer()
    {
        return server;
    };
};