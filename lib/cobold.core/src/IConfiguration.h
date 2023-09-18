#pragma once

#include <string>
#include <vector>
#include <map>

namespace cobold
{
    namespace configuration
    {

        class IConfiguration
        {
        public:
            virtual ~IConfiguration() {}

            virtual std::string getValue(const std::string &key, const std::string &defaultValue = "") = 0;

            virtual void setValue(const std::string &key, const std::string &value) = 0;

            virtual void deleteKey(const std::string &key) = 0;

            virtual bool keyExists(const std::string &key) = 0;

            virtual std::string findKeyStartingWith(const std::string &prefix) = 0;

            virtual std::vector<std::string> findKeysByRegex(const std::string &regexPattern) = 0;

            virtual IConfiguration *getSection(const std::string &path) = 0;

            /**
             * @brief Update the configuration with the given configuration
             *
             * @details This method use all pairs from the given configuration to update the current configuration.
             */
            virtual void update(IConfiguration *configuration) = 0;

            virtual std::string toJson() = 0;
            virtual std::map<std::string, std::string> toMap() = 0;
        };

    } // namespace configuration

}