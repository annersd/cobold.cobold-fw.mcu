#pragma once

#include "ServiceCollection.h"
#include "IHostBuilder.h"
#include "IHost.h"

namespace cobold
{
    class IApplication
    {
    public:
        virtual ~IApplication() {}

        virtual void preSetup() = 0;
        virtual void setup() = 0;
        virtual void loop() = 0;

        virtual ServiceCollection *getServices() = 0;
        virtual cobold::hosting::IHost *getHost() = 0;
        virtual cobold::hosting::IHostBuilder *getHostBuilder() = 0;
    };

    cobold::IApplication *app = nullptr;
} // namespace cobold

