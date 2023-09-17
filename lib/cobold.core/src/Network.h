#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include "ArduinoJson.h"

#include "Logger.h"
#include "IApplication.h"

/**
 * @brief Class for managing the network connection.
 *
 * This class is responsible for connecting to the IP network.
 *
 * @todo Add support for Ethernet
 * @todo Handle WiFi connection errors carefully, especially how often to try to reconnect
 */
class Network
{
private:
    String ssid;
    String password;
    cobold::Logger *logger;
    cobold::IApplication *app;
    TimerHandle_t wifiReconnectTimer;

    void connectToWifi()
    {
        Serial.println("Connecting to Wi-Fi...");
        WiFi.begin(ssid, password);
    }

    void WiFiEvent(WiFiEvent_t event, WiFiEventInfo_t info)
    {
        switch (event)
        {
        case SYSTEM_EVENT_WIFI_READY:
            logger->info("[WiFi-event] WiFi interface ready");
            break;
        case SYSTEM_EVENT_SCAN_DONE:
            logger->info("[WiFi-event] Completed scan for access points");
            break;
        case SYSTEM_EVENT_STA_START:
            logger->info("[WiFi-event] WiFi client started");
            break;
        case SYSTEM_EVENT_STA_STOP:
            logger->info("[WiFi-event] WiFi clients stopped");
            // start the timer to try to reconnect to WiFi every in 2 seconds
            xTimerStart(wifiReconnectTimer, 0);
            break;
        case SYSTEM_EVENT_STA_CONNECTED:
            logger->info("[WiFi-event] Connected to access point");
            break;
        case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
            logger->info("[WiFi-event] Authentication mode of access point has changed");
            break;
        case SYSTEM_EVENT_STA_GOT_IP:
            logger->info("[WiFi-event] WiFi connected; IP address: %s", WiFi.localIP().toString());
            app->raiseEvent(cobold::sys::Event::create("cobold.network.connected", "void", ""));
            break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
            logger->info("[WiFi-event] WiFi lost connection");
            // start the timer to try to reconnect to WiFi every in 2 seconds
            xTimerStart(wifiReconnectTimer, 0);
            break;
        }
    }

public:
    Network(cobold::IApplication *app, String ssid, String password){
         this->ssid = ssid;
    this->password = password;
    this->app = app;

    wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void *)0, [](TimerHandle_t xTimer) -> void
                                      {
                                          Network *self = static_cast<Network *>(pvTimerGetTimerID(xTimer));
                                          Serial.println("Trying to reconnect to WiFi...");
                                          if (self != nullptr)
                                          {
                                              self->connectToWifi();
                                          }
                                      });
    }

    void setup()
    {
        logger->debug("Starting WiFi setup");

        WiFi.mode(WIFI_STA);

        logger->debug("Connecting to %s", ssid);
        logger->debug("Password: %s", password);

        WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info)
                     { this->WiFiEvent(event, info); });

        connectToWifi();
        if (WiFi.waitForConnectResult() != WL_CONNECTED)
        {
            logger->fatal("WiFi Failed!");
            return;
        }

        logger->debug("WiFi connected");
        logger->debug("IP address: %s", WiFi.localIP().toString().c_str());
    };
};