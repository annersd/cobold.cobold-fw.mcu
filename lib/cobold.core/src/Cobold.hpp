#pragma once

#define COBOLD_CORE_VERSION_MAJOR 0
#define COBOLD_CORE_VERSION_MINOR 1
#define COBOLD_CORE_VERSION_PATCH 0

#include <Arduino.h>
#include <ArduinoJson.h>
#include <ArduinoLog.h>

#include <vector>
#include <string>
#include <map>
#include <functional>
#include <regex>

#include "Object.h"
#include "Event.h"
#include "Logger.h"
#include "IApplication.h"
#include "ServiceCollection.h"
#include "ServiceCollectionExtensions.h"
#include "Options.h"


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
#include "Component.h"
#include "DynamicComponent.h"

#include "Scheduler.h"
#include "EventDispatcher.h"

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

#include "ArduinoJson.h"
#include "AsyncMqttClient.h"

#include "Network.h"

#include "ComponentSvc.h"

namespace cobold
{
    cobold::IApplication *app;
} // namespace cobold


namespace cobold::components
{
    template <typename TComponet>
    TComponet *getComponent(const char *identifier)
    {
        auto component = cobold::app->getServices()->getService<ComponentSvc>()
            ->getComponent(identifier);
        return dynamic_cast<TComponet *>(component);
    }
} // namespace cobold::components

namespace cobold::services
{
    template <typename TService>
    TService *getService()
    {
        return cobold::app->getServices()->getService<TService>();
    }
} // namespace cobold::services 