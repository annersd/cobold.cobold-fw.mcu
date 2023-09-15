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
            virtual void *getObject() = 0;

            /**
             * @brief Get the type name of the wrapped object.
             *
             * @return The type name of the wrapped object.
             */
            virtual std::string getTypeName() = 0;

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
         * 
         * @example
         * 
         * // Wrap an object
         * std::string *str = new std::string("Hello");
         * cobold::sys::Object<std::string> *wrappedStr = new cobold::sys::Object<std::string>(str, true);
         * 
         * // Unwrap the object
         * std::string *unwrappedStr = cobold::sys::unwrap(wrappedStr);
         * 
         * // Automatic delete on destruction
         * cobold::sys::Object<std::string> *wrappedStr2 
         *      = new cobold::sys::Object<std::string>(new std::string("Hello"), true);
         * delete wrappedStr2; // The wrapped string will be deleted automatically
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

            void *getObject()  override
            {
                return (void*) (object);
            }

            T *get() 
            {
                return object;
            }

            std::string getTypeName()  override
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

        template <typename T>
        static T *unwrap(BaseObject *obj, T *defaultValue = nullptr)
        {
            if (obj == nullptr)
            {
                // Handle the case when 'obj' is null, e.g., by returning a default value or throwing an exception.
                return defaultValue;
            }

            Object<T> *typedObj = dynamic_cast<Object<T> *>(obj);
            if (typedObj == nullptr)
            {
                // Handle the case when the dynamic type doesn't match 'T', e.g., by returning a default value or throwing an exception.
                return defaultValue;
            }

            return typedObj->get();
        }

        template <typename T>
        static T *unwrap(BaseObject &obj, T *defaultValue = nullptr)
        {
            return unwrap(&obj, defaultValue);
        }

        template <typename T>
        static T *unwrap(BaseObject &&obj, T *defaultValue = nullptr)
        {
            return unwrap(&obj, defaultValue);
        }

        template <typename T>
        static cobold::sys::Object<T> *wrap(T *obj, bool shouldDelete = false)
        {
            return new cobold::sys::Object<T>(obj, shouldDelete);
        }

        template <typename T>
        static cobold::sys::Object<T> *wrap(T &obj, bool shouldDelete = false)
        {
            return new cobold::sys::Object<T>(&obj, shouldDelete);
        }

        template <typename T>
        static cobold::sys::Object<T> *wrap(T &&obj, bool shouldDelete = false)
        {
            return new cobold::sys::Object<T>(&obj, shouldDelete);
        }

        template <typename T>
        static cobold::sys::Object<T> *wrap(T *obj, bool shouldDelete, std::map<std::string, std::string> properties)
        {
            cobold::sys::Object<T> *wrappedObj = new cobold::sys::Object<T>(obj, shouldDelete);
            for (auto it = properties.begin(); it != properties.end(); ++it)
            {
                wrappedObj->setProperty(it->first, it->second);
            }
            return wrappedObj;
        }
        
    } // namespace sys
} // namespace cobold