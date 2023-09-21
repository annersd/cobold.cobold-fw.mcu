#pragma once

#include "IComponent.h"
#include <string>

namespace cobold::components
{

    /**
     * @class Component
     * @brief The Component class is a base class for physical components that require
     *        update and initialization behavior.
     */
    class Component : public IComponent
    {
    protected:
        // The display name of the component
        std::string name_;

        // The unique identifier of the component
        std::string identifier_;

    public:
        /**
         * @brief Construct a new Component object
         */
        Component() = default;

        /**
         * @brief Destroy the Component object
         */
        virtual ~Component() = default;

        /**
         * @brief Initialize the component
         */
        virtual void initialize() override
        {
            initialize_impl();
        }

        /**
         * @brief Update the component
         */
        virtual void update() override
        {
            update_impl();
        }

        /**
         * @brief Configure the component
         */
        virtual void configure() override
        {
            configure_impl();
        }

        /**
         * @brief Get the name of the component
         * @return std::string
         */
        virtual std::string getName() const override
        {
            return name_;
        }

        /**
         * @brief Get the identifier of the component
         * @return std::string
         */

        virtual std::string getId() const override
        {
            return identifier_;
        }

        void setName(std::string name)
        {
            name_ = name;
        }

        void setId(std::string id)
        {
            identifier_ = id;
        }
        

    protected:
        /**
         * @brief Initialize the component
         */
        virtual void initialize_impl(){};

        /**
         * @brief Update the component
         */
        virtual void update_impl(){};

        /**
         * @brief Configure the component
         */
        virtual void configure_impl(){};
    };
}