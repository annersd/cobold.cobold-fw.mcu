#pragma once

#include <Arduino.h>    // Include the Arduino library
#include "Configuration.h"
#include "HostingExtensions.h"
#include "Logger.h"

namespace cobold
{
    namespace hosting
    {

        class Host : public IHost
        {
        private:
            /* data */
            ServiceCollection *serviceCollection;
            Logger *logger;

        public:
            explicit Host(ServiceCollection *serviceCollection);
            ~Host();
            void start() override;
            void stop() override;
            ServiceCollection *getServices();
        };

        Host::Host(ServiceCollection *serviceCollection)
        {
            this->serviceCollection = serviceCollection;
            logger = serviceCollection->getService<Logger>();
        }

        Host::~Host()
        {
        }

        void Host::start()
        {
            logger->verbose("Host::start()");
            // get all the services that implement IHostedService
            auto hostedServices = cobold::hosting::HostingExtensions::GetHostedServices(serviceCollection);

            for (auto hostedService : hostedServices)
            {
                logger->verbose("Host::start() - starting hosted service");
                hostedService->start();
            }
            logger->verbose("Host::start() - done");
        }

        void Host::stop()
        {
            // get all the services that implement IHostedService
            auto hostedServices = cobold::hosting::HostingExtensions::GetHostedServices(serviceCollection);

            for (auto hostedService : hostedServices)
            {
                hostedService->stop();
            }
        }

        ServiceCollection *Host::getServices()
        {
            return serviceCollection;
        }

    } // namespace hosting
} // namespace cobold