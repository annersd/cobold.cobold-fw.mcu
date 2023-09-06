#pragma once

#include "ServiceCollection.h"

namespace cobold
{
    namespace hosting
    {

        /**
         * @brief Interface for a host that manages services and their lifecycle.
         */
        class IHost
        {
        public:
            /**
             * @brief Start the host and its associated services.
             */
            virtual void start() = 0;

            /**
             * @brief Stop the host and its associated services.
             */
            virtual void stop() = 0;

            /**
             * @brief Get the collection of services managed by the host.
             *
             * @return A pointer to the ServiceCollection instance.
             */
            virtual ServiceCollection *getServices() = 0;
        };

    }
}