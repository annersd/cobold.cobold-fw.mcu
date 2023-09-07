#pragma once

#include <Arduino.h>
#include "Logger.h"

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

class Network
{
private:
    const char *ssid = nullptr;
    const char *password = nullptr;
    cobold::Logger *logger;

public:
    Network(const char *ssid, const char *password);

    void setup()
    {
        logger->debug("Starting WiFi setup");

        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, password);
        if (WiFi.waitForConnectResult() != WL_CONNECTED)
        {
            logger->fatal("WiFi Failed!");
            return;
        }

        logger->debug("WiFi connected");
        logger->debug("IP address: %s", WiFi.localIP().toString().c_str());
    };
};