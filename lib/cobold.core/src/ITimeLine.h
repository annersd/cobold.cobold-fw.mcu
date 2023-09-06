#pragma once

#include "IComponent.h"
#include <stdint.h>
#include <chrono>

namespace cobold {
    namespace time {

        /**
         * @brief Alias for a 64-bit unsigned integer representing timeline values.
         * 
         * The timeline works with a precision of microseconds since year 0.
         * - 1 January 0000 00:00:00 UTC is represented by the value 0.
         * - 1 January 1970 00:00:00 UTC is represented by the value 62135596800000000.
         * - 1 January 2000 00:00:00 UTC is represented by the value 94668480000000000.
         */
        typedef uint64_t timeline_t;

        /**
         * @brief Interface for timeline management.
         *
         * This class defines an interface for managing timelines used for time measurement.
         * It extends the `IComponent` interface.
         */
        class ITimeline : public cobold::components::IComponent {

        public:
            /**
             * @brief Default constructor.
             */
            ITimeline() = default;

            /**
             * @brief Destructor.
             */
            virtual ~ITimeline() = default;

            /**
             * @brief Update method for the timeline.
             *
             * This method should be implemented in derived classes to perform any necessary updates
             * to the timeline. By default, it does nothing.
             */
            void update() override {
            }

            /**
             * @brief Initialize the timeline.
             *
             * This method should be implemented in derived classes to perform any necessary initialization
             * of the timeline. By default, it does nothing.
             */
            void initialize() override {
            }

            /**
             * @brief Get the current time value from the timeline.
             *
             * This pure virtual method should be implemented in derived classes to return the current time
             * value from the timeline.
             *
             * @return The current time value.
             */
            virtual timeline_t now() = 0;
        };

    }
}
