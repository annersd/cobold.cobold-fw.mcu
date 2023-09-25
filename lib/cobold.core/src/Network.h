#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include "ArduinoJson.h"

#include "Logger.h"
#include "IApplication.h"
#include "IConfiguration.h"

namespace cobold::sys
{
    class NetworkChangedEventArgs : public EventArgs
    {
    public:
        NetworkChangedEventArgs(bool connected)
        {
            this->connected = connected;
        }

        bool connected;

        const char *getType() const override { return "net.cobold.services.network.changed"; }

        std::string toJson(bool pretty = false) override
        {
            StaticJsonDocument<256> doc;
            doc["connected"] = connected;
            std::string json;
            serializeJson(doc, json);
            return json;
        }

        bool fromJson(std::string json) override
        {
            StaticJsonDocument<256> doc;
            deserializeJson(doc, json);
            connected = doc["connected"];

            return true;
        }
    };

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
        cobold::SerialLogger *logger;
        cobold::IApplication *app;
        TimerHandle_t wifiReconnectTimer;

        void connectToWifi()
        {
            logger->info("[Network] Connecting to %s", ssid.c_str());
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
                app->raiseEvent("cobold.network.connected", new NetworkChangedEventArgs(true));
                break;
            case SYSTEM_EVENT_STA_DISCONNECTED:
                logger->info("[WiFi-event] WiFi lost connection");
                // start the timer to try to reconnect to WiFi every in 2 seconds
                xTimerStart(wifiReconnectTimer, 0);
                break;
            }
        }

    public:
        Network(cobold::IApplication *app, cobold::configuration::IConfiguration *wifiSettings)
        {
            this->ssid = wifiSettings->getValue("ssid").c_str();
            this->password = wifiSettings->getValue("password").c_str();
            this->app = app;
        }

        void setup()
        {
            logger = app->getServices()->getService<cobold::SerialLogger>();

            wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void *)0, [](TimerHandle_t xTimer) -> void
                                              {
                                          Network *self = static_cast<Network *>(pvTimerGetTimerID(xTimer));
                                          if (self != nullptr)
                                          {
                                              self->connectToWifi();
                                          } });

            logger->debug("[Network] Starting WiFi setup");

            WiFi.mode(WIFI_STA);

            // logger->debug("Connecting to %s", ssid);
            // logger->debug("Password: %s", password);

            WiFi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info)
                         { this->WiFiEvent(event, info); });

            connectToWifi();
            if (WiFi.waitForConnectResult() != WL_CONNECTED)
            {
                logger->fatal("[Network] WiFi Failed!");
                return;
            }

            logger->debug("[Network] WiFi connected");
            logger->debug("[Network] IP address: %s", WiFi.localIP().toString().c_str());
        };
    };

} // namespace cobold::sys