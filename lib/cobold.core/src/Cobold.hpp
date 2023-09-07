#pragma once

#define COBOLD_CORE_VERSION_MAJOR 0
#define COBOLD_CORE_VERSION_MINOR 1
#define COBOLD_CORE_VERSION_PATCH 0

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoLog.h>

#include "Logger.h"
#include "IApplication.h"
#include "ServiceCollection.h"
#include "ServiceCollectionExtensions.h"

// Include Hosting
#include "IHost.h"
#include "IHostBuilder.h"
#include "IHostedService.h"
#include "HostingExtensions.h"

// Include Configuration
#include "IConfiguration.h"
#include "Configuration.h"

#include "IComponent.h"
#include "ComponentExtensions.h"

// possible link error 
// #include "BigInt128.h"

#include "ITimeLine.h"

#include "Application.h"

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

#include "Network.h"
#include "WebServer.h"


namespace cobold
{
    cobold::IApplication *app;
} // namespace cobold
