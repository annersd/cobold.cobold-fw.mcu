#pragma once

#include "IConfiguration.h"

namespace cobold::sys
{
    /**
     * @brief The Options class read the configuration and provides the coresponding values
    */
    class Options
    {
    private:

    public:
        Options()
        {
            this->configuration = nullptr;
        };
        Options(cobold::configuration::IConfiguration *configuration)
        {
            this->parseConfiguration(configuration);
            this->configuration = configuration;
        };
        ~Options() = default;

        /**
         * @brief Update the Options with a new configuration
        */
        void updateConfiguration(cobold::configuration::IConfiguration *configuration)
        {
            this->parseConfiguration(configuration);
            this->configuration = configuration;
        };

        void log()
        {
            this->logOptions();
        };

    protected:
        virtual void parseConfiguration(cobold::configuration::IConfiguration *configuration) {};
        virtual void logOptions() {};
        cobold::configuration::IConfiguration *configuration = nullptr;
    };

}