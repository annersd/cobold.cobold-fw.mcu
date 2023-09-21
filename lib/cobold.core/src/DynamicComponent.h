#pragma once

#include "Component.h"
#include <functional>

namespace cobold::components
{

    /**
     * @class DynamicComponent
     * @brief The DynamicComponent class is a base class for physical components that require
     *        update and initialization behavior.
     */
    class DynamicComponent : public Component
    {
        private:
        std::function<void(DynamicComponent*)> initialize_func;
        std::function<void(DynamicComponent*)> update_func;
        std::function<void(DynamicComponent*)> configure_func;
    public:
        
        /**
         * @brief Construct a new DynamicComponent object
         */
        DynamicComponent() = default;

        /**
         * @brief Destroy the DynamicComponent object
         */
        virtual ~DynamicComponent() = default;

        virtual void initialize_impl() override
        {
            initialize_func(this);
        }

        virtual void update_impl() override
        {
            update_func(this);
        }

        virtual void configure_impl() override
        {
            configure_func(this);
        }

        void bind_initialize(std::function<void(DynamicComponent*)> initialize)
        {
            this->initialize_func = initialize;
        }

        void bind_update(std::function<void(DynamicComponent*)> update)
        {
            this->update_func = update;
        }

        void bind_configure(std::function<void(DynamicComponent*)> configure)
        {
            this->configure_func = configure;
        }

             
    };

} // namespace cobold::components