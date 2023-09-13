#pragma once

#include <typeinfo>
#include <string>
#include <map>
#include "XxHash_arduino.h"

namespace cobold
{
    namespace sys
    {
        class BaseObject
        {
        public:
            /**
             * @brief Get the wrapped object.
             */
            virtual void *getObject() const = 0;

            /**
             * @brief Get the type name of the wrapped object.
             *
             * @return The type name of the wrapped object.
             */
            virtual std::string getTypeName() const = 0;

            /**
             * @brief Get the hash of the object.
             */
            virtual uint32_t getHash() = 0;

            /**
             * @brief Get the value of the specified property.
             */
            virtual ~BaseObject() = default;

            /**
             * @brief Get the value of the specified property.
             *
             * @param key The key of the property.
             * @return The value of the property.
             */
            virtual std::string getProperty(const std::string &key) const = 0;

            /**
             * @brief Set the value of the specified property.
             *
             * @param key The key of the property.
             * @param value The value of the property.
             */
            virtual void setProperty(const std::string &key, const std::string &value) = 0;
        };

        /**
         * @brief A class for wrapping an object.
         *
         * @tparam T The type of the object to wrap.
         */
        template <typename T>
        class Object : public BaseObject
        {
        private:
            T *object;
            bool shouldDelete; // Flag to indicate whether to delete the wrapped object
            std::map<std::string, std::string> properties;

        public:
            Object(T *object, bool shouldDelete = false) : object(object), shouldDelete(shouldDelete) {}

            ~Object()
            {
                if (shouldDelete && object != nullptr)
                {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdelete-incomplete"
                    delete object;
#pragma GCC diagnostic pop
                }
            }

            void *getObject() const override
            {
                return object;
            }

            std::string getTypeName() const
            {
                return typeid(T).name();
            }

            uint32_t getHash() override
            {
                const char *data = getTypeName().c_str();
                size_t dataSize = strlen(data);

                // Compute the 32-bit hash
                return XXH32(data, dataSize, 0);
            }

            /**
             * @brief Get a property value by key.
             *
             * @param key The key of the property to retrieve.
             * @return The value of the property, or an empty string if not found.
             */
            std::string getProperty(const std::string &key) const override
            {
                auto it = properties.find(key);
                if (it != properties.end())
                {
                    return it->second;
                }
                return "";
            }

            /**
             * @brief Set a property value by key.
             *
             * @param key The key of the property to set.
             * @param value The value of the property.
             */
            void setProperty(const std::string &key, const std::string &value) override
            {
                properties[key] = value;
            }
        };
    } // namespace sys
} // namespace cobold