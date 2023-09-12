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

        /**
         * @brief Implement your preSetup logic here
         * 
         * @note This method is called before the setup() method
        */
        virtual void preSetup() = 0;

        /**
         * @brief Implement your setup logic here
         * 
         * @note This method is called after the preSetup() method
        */
        virtual void setup() = 0;

        /**
         * @brief The main loop of the application
        */
        virtual void loop() = 0;

        /**
         * @brief Run the application
        */
        virtual void run() = 0;

        virtual void dispatch(std::function<void()> function) = 0;
        virtual void raiseEvent(std::string name, void* data) = 0;

        virtual ServiceCollection *getServices() = 0;
        virtual cobold::hosting::IHost *getHost() = 0;
        virtual cobold::hosting::IHostBuilder *getHostBuilder() = 0;
        virtual cobold::configuration::IConfiguration *getAppConfiguration() = 0;
    };

    // cobold::IApplication *app = nullptr;
} // namespace cobold

