
#pragma once

namespace cobold
{

    namespace components
    {

        /**
         * @class IComponent
         * @brief The IComponent interface defines a contract for components that require update and initialization behavior.
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
             * Implementing classes should perform necessary setup or initialization tasks.
             */
            virtual void initialize() = 0;
        };
    }
}
