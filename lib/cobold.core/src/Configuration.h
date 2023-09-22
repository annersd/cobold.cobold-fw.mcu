#pragma once

#include <map>
#include <string>
#include <vector>
#include <regex>
#include <ArduinoJson.h>

namespace cobold
{
    namespace configuration
    {
        class Configuration : public cobold::configuration::IConfiguration
        {
        protected:
            std::map<std::string, std::string> configMap;

        public:
            Configuration()
            {
            }

            std::string getValue(const std::string &key, const std::string &defaultValue = "")
            {
                auto it = configMap.find(key);
                if (it != configMap.end())
                {
                    return it->second;
                }
                else
                {
                    return defaultValue;
                }
            }

            int getInt(const std::string &key, int defaultValue = 0)
            {
                auto it = configMap.find(key);
                if (it != configMap.end())
                {
                    return std::stoi(it->second);
                }
                else
                {
                    return defaultValue;
                }
            }

            float getFloat(const std::string &key, float defaultValue = 0.0f)
            {
                auto it = configMap.find(key);
                if (it != configMap.end())
                {
                    return std::stof(it->second);
                }
                else
                {
                    return defaultValue;
                }
            }

            bool getBool(const std::string &key, bool defaultValue = false)
            {
                auto it = configMap.find(key);
                if (it != configMap.end())
                {
                    return it->second == "true";
                }
                else
                {
                    return defaultValue;
                }
            }

            std::string getString(const std::string &key, const std::string &defaultValue = "")
            {
                auto it = configMap.find(key);
                if (it != configMap.end())
                {
                    return it->second;
                }
                else
                {
                    return defaultValue;
                }
            }

            void setValue(const std::string &key, const std::string &value)
            {
                configMap[key] = value;
            }

            void setInt(const std::string &key, int value)
            {
                configMap[key] = std::to_string(value);
            }          

            void setFloat(const std::string &key, float value)
            {
                configMap[key] = std::to_string(value);
            }

            void setBool(const std::string &key, bool value)
            {
                configMap[key] = value ? "true" : "false";
            }

            void setString(const std::string &key, const std::string &value)
            {
                configMap[key] = value;
            }
            
              

            void deleteKey(const std::string &key)
            {
                configMap.erase(key);
            }

            bool keyExists(const std::string &key)
            {
                return configMap.find(key) != configMap.end();
            }

            std::string findKeyStartingWith(const std::string &prefix)
            {
                for (const auto &pair : configMap)
                {
                    if (pair.first.find(prefix) == 0)
                    {
                        return pair.first;
                    }
                }
                return "";
            }

            std::vector<std::string> findKeysByRegex(const std::string &regexPattern)
            {
                std::vector<std::string> matchingKeys;
                std::regex pattern(regexPattern);

                for (const auto &pair : configMap)
                {
                    if (std::regex_match(pair.first, pattern))
                    {
                        matchingKeys.push_back(pair.first);
                    }
                }

                return matchingKeys;
            }

            cobold::configuration::IConfiguration *getSection(const std::string &path) override
            {
                cobold::configuration::IConfiguration *sectionConfig = new Configuration();

                // Serial.println("getSection");

                std::string regexPattern = path + "\\..*";
                std::regex pattern(regexPattern);

                std::string prefix = path + ".";
                size_t prefixLength = prefix.length();

                for (const auto &pair : configMap)
                {

                    // Serial.println(pair.first.c_str());
                    // Serial.println(pair.second.c_str());
                    if (std::regex_match(pair.first, pattern))
                    {
                        std::string shortenedKey = pair.first.substr(prefixLength);

                        sectionConfig->setValue(shortenedKey, pair.second);
                    }
                }

                return sectionConfig;
            }

            void update(IConfiguration *configuration) override
            {
                // Serial.println("update");
                for (const auto &pair : configuration->toMap())
                {
                    // Serial.println(pair.first.c_str());
                    // Serial.println(pair.second.c_str());
                    this->setValue(pair.first, pair.second);
                }
            }

            void loadFromJson(const char *jsonString, const std::string &prefix = "")
            {
                StaticJsonDocument<1024> jsonDoc;
                deserializeJson(jsonDoc, jsonString);

                for (const JsonPair &kvp : jsonDoc.as<JsonObject>())
                {
                    std::string key = prefix.empty() ? kvp.key().c_str() : prefix + "." + kvp.key().c_str();

                    if (kvp.value().is<JsonObject>())
                    {
                        // Recursively load nested JSON objects
                        loadFromJson(kvp.value().as<std::string>().c_str(), key);
                    }
                    else
                    {
                        // Set the value for the current key
                        std::string value = kvp.value().as<std::string>();
                        setValue(key, value);
                        // Serial.println(key.c_str());
                        // Serial.println(value.c_str());
                    }
                }
            }

            std::string toJson() override
            {
                StaticJsonDocument<4096> jsonDoc;

                for (const auto &pair : configMap)
                {
                    const std::string &key = pair.first;
                    const std::string &value = pair.second;

                    // Split the key into segments based on '.'
                    std::vector<std::string> keySegments;
                    std::istringstream keyStream(key);
                    std::string segment;

                    while (std::getline(keyStream, segment, '.'))
                    {
                        keySegments.push_back(segment);
                    }

                    // Create a nested JSON structure based on key segments
                    JsonObject jsonObject = jsonDoc.to<JsonObject>();
                    for (size_t i = 0; i < keySegments.size() - 1; ++i)
                    {
                        jsonObject = jsonObject.createNestedObject(keySegments[i]);
                    }

                    // Set the value in the nested JSON structure
                    jsonObject[keySegments.back()] = value;
                }

                std::string jsonString;
                serializeJson(jsonDoc, jsonString);

                return jsonString;
            }

            std::map<std::string, std::string> toMap() override
            {
                return configMap;
            }

            static IConfiguration* fromJson(const char *jsonString)
            {
                Configuration *config = new Configuration();
                config->loadFromJson(jsonString);
                return config;
            } 
        };

    }
}