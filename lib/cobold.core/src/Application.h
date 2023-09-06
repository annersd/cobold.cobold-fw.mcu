#pragma once

#include "IApplication.h"
#include "ServiceCollection.h"
#include "IHost.h"
#include "IHostBuilder.h"


namespace cobold
{

    class Application : public cobold::IApplication
    {
    public:
        Application();
        ~Application();

        void preSetup() ;
        void setup() ;
        void loop() ;

        ServiceCollection *getServices();
        cobold::hosting::IHost *getHost() ;
        cobold::hosting::IHostBuilder *getHostBuilder() ;

    private:
        ServiceCollection *services;
        cobold::hosting::IHost *host;
        cobold::hosting::IHostBuilder *hostBuilder;
    };

    //Application *app = new Application();
} // namespace cobold
