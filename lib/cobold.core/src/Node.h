#pragma once

#include <string>
#include "IApplication.h"
#include "IConfiguration.h"

namespace cobold
{
    /**
     * @brief A node represents a logical unit in the cobold network.
     */
    class Node
    {
    private:
        /* data */
        std::string name;
        IApplication *app;
        cobold::configuration::IConfiguration *config;

    public:
        Node(IApplication *app, cobold::configuration::IConfiguration *config)
        {
            this->app = app;
            this->config = config;

            name = config->getValue("name");
        };
        ~Node() = default;

        void setup(){

        };
    };
}