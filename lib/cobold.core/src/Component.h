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
    private:
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
        virtual void initialize() override;

        /**
         * @brief Update the component
         */
        virtual void update() override;

        /**
         * @brief Configure the component
         */
        virtual void configure() override;

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

    protected:
        /**
         * @brief Initialize the component
         */
        virtual void initialize_impl() = 0;

        /**
         * @brief Update the component
         */
        virtual void update_impl() = 0;

        /**
         * @brief Configure the component
         */
        virtual void configure_impl() = 0;
    };
}