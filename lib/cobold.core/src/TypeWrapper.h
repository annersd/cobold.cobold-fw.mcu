#pragma once

#include <string>
#include <map>
#include "ITypeWrapper.h"

/**
 * @brief Template class to generate unique type IDs, type names, and provide access to wrapped types for TypeWrapper instances.
 *
 * @tparam T The type for which to generate the type ID and name.
 */
template <typename T>
class TypeWrapper : public ITypeWrapper
{
    private:
    std::map<std::string, std::string> properties;
public:
    /**
     * @brief Get the mangled type name for the specified type.
     *
     * @return The mangled type name.
     */
    std::string GetTypeName() const override
    {
        return typeid(T).name();
    }

    /**
     * @brief Get the unique type ID for the specified type.
     *
     * This method uses the mangled type name as the basis for the type identifier.
     *
     * @return The unique type ID.
     */
    size_t GetTypeId() const override
    {
        static const size_t id = std::hash<std::string>{}(GetTypeName());
        return id;
    }

    /**
     * @brief Get the name of the wrapped type.
     *
     * @return The name of the wrapped type.
     */
    std::string GetName() const override
    {
        return GetTypeName();
    }

    /**
     * @brief Get the type information of the wrapped type.
     *
     * @return The type information of the wrapped type.
     */
    const std::type_info &GetWrappedType() const override
    {
        return typeid(T);
    }


    /**
     * @brief Get a property value by key.
     *
     * @param key The key of the property to retrieve.
     * @return The value of the property, or an empty string if not found.
     */
    std::string GetProperty(const std::string &key) const override
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
    void SetProperty(const std::string &key, const std::string &value) override
    {
        properties[key] = value;
    }
};
