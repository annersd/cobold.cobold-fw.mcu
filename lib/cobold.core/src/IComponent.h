
#pragma once

#include <string>

namespace cobold
{

    namespace components
    {

        /**
         * @class IComponent
         * @brief The IComponent interface defines a contract for physical components that require
         *        update and initialization behavior.
         */
        class IComponent
        {
        public:
            virtual ~IComponent() {}

            /**
             * @brief Update the component's state or perform periodic tasks.
             * Implementing classes should define the behavior of this method.
             */
            virtual void update() = 0;

            /**
             * @brief Initialize the component.
             * Implementing classes should perform necessary initialization tasks.
             */
            virtual void initialize() = 0;

            /**
             * @brief Configure the component.
             * Implementing classes should perform necessary configuration tasks.
             */
            virtual void configure() = 0;

            /**
             * @brief Get the name of the component.
             * Implementing classes should return a string that identifies the component.
             * @return std::string
             */
            virtual std::string getName() const = 0;

            virtual std::string getId() const = 0;
        };
    }
}
