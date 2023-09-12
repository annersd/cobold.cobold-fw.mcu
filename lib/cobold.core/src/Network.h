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
    String ssid;
    String password;
    cobold::Logger *logger;

    static void WiFiEvent(WiFiEvent_t event)
    {
        Serial.printf("[WiFi-event] event: %d\n", event);
        switch (event)
        {
        case SYSTEM_EVENT_WIFI_READY:
            Serial.println("WiFi interface ready");
            break;
        case SYSTEM_EVENT_SCAN_DONE:
            Serial.println("Completed scan for access points");
            break;
        case SYSTEM_EVENT_STA_START:
            Serial.println("WiFi client started");
            break;
        case SYSTEM_EVENT_STA_STOP:
            Serial.println("WiFi clients stopped");
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            Serial.println("Connected to access point");
            break;

        case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
            Serial.println("Authentication mode of access point has changed");
            break;

        case SYSTEM_EVENT_STA_GOT_IP:
            Serial.println("WiFi connected");
            Serial.println("IP address: ");
            Serial.println(WiFi.localIP());
            // connectToMqtt();
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            Serial.println("WiFi lost connection");
            // xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
            // xTimerStart(wifiReconnectTimer, 0);
            break;
        }
    }

public:
    Network(String ssid, String password);

    void setup()
    {
        logger->debug("Starting WiFi setup");

        WiFi.mode(WIFI_STA);

        logger->debug("Connecting to %s", ssid);
        logger->debug("Password: %s", password);

        WiFi.onEvent(WiFiEvent);

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