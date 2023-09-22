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

            virtual int getInt(const std::string &key, int defaultValue = 0) = 0;

            virtual float getFloat(const std::string &key, float defaultValue = 0.0f) = 0;

            virtual bool getBool(const std::string &key, bool defaultValue = false) = 0;

            virtual std::string getString(const std::string &key, const std::string &defaultValue = "") = 0;

            virtual void setValue(const std::string &key, const std::string &value) = 0;

            virtual void setInt(const std::string &key, int value) = 0;

            virtual void setFloat(const std::string &key, float value) = 0;

            virtual void setBool(const std::string &key, bool value) = 0;

            virtual void setString(const std::string &key, const std::string &value) = 0;

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