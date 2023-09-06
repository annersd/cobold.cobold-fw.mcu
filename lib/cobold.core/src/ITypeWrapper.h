#pragma once

/**
 * @brief Interface for generating unique type IDs, type names, and accessing wrapped types.
 */
class ITypeWrapper
{
public:
    /**
     * @brief Get the mangled type name for the specified type.
     *
     * @return The mangled type name.
     */
    virtual std::string GetTypeName() const = 0;

    /**
     * @brief Get the unique type ID for the specified type.
     *
     * This method uses the mangled type name as the basis for the type identifier.
     *
     * @return The unique type ID.
     */
    virtual size_t GetTypeId() const = 0;

    /**
     * @brief Get the name of the wrapped type.
     *
     * @return The name of the wrapped type.
     */
    virtual std::string GetName() const = 0;

    /**
     * @brief Get the type information of the wrapped type.
     *
     * @return The type information of the wrapped type.
     */
    virtual const std::type_info& GetWrappedType() const = 0;


    virtual ~ITypeWrapper() {}

    /**
     * @brief Get the value of the specified property.
     *
     * @param key The key of the property.
     * @return The value of the property.
     */
    virtual std::string GetProperty(const std::string &key) const = 0;

    /**
     * @brief Set the value of the specified property.
     *
     * @param key The key of the property.
     * @param value The value of the property.
     */
    virtual void SetProperty(const std::string &key, const std::string &value) = 0;
};