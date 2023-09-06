#pragma once

#include "IConfiguration.h"
#include "IHost.h"

namespace cobold
{
    namespace hosting
    {

        /**
         * @brief Interface for a host builder that configures and builds an IHost instance.
         */
        class IHostBuilder
        {
        public:
            /**
             * @brief Configure the application's configuration using a lambda function.
             *
             * @param configLambda The lambda function to configure the application's configuration.
             * @return A pointer to the IHostBuilder instance.
             */
            virtual IHostBuilder *configureAppConfiguration(std::function<void(cobold::configuration::IConfiguration *)> configLambda) = 0;

            /**
             * @brief Configure the services using a lambda function.
             *
             * @param serviceConfigLambda The lambda function to configure the services.
             * @return A pointer to the IHostBuilder instance.
             */
            virtual IHostBuilder *configureServices(std::function<void(ServiceCollection *)> serviceConfigLambda) = 0;

            /**
             * @brief Build an IHost instance with the configured settings.
             *
             * @return A pointer to the built IHost instance.
             */
            virtual cobold::hosting::IHost *build() = 0;
        };
    }
}