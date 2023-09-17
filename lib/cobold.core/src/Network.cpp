#include "Network.h"

#include "IApplication.h"
#include "Logger.h"

Network::Network(cobold::IApplication *app, String ssid, String password)
{
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