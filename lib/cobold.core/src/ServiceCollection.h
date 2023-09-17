#pragma once

#include <Arduino.h>    // Include the Arduino library


#include <map>
#include <functional>


#include "Object.h"
#include "Logger.h"

/**
 * @brief Class for managing a collection of services using dependency injection.
 */
class ServiceCollection
{
public:
    ServiceCollection(cobold::Logger *logger)
    {
        this->logger = logger;
        logger->verbose("ServiceCollection constructor");
        services = {};
        constructorMap = {};
    }

    /**
     * @brief Add a service to the collection without a constructor.
     *
     * The service can be later retrieved using the getService method.
     *
     * @tparam T The type of the service to add.
     */
    template <typename T>
    void addService()
    {
        services[new cobold::sys::Object<T>()] = []() -> void *
        { return new T(); };
    }

    /**
     * @brief Add a service to the collection with a constructor.
     *
     * The service can be later retrieved using the getService method.
     *
     * @tparam T The type of the service to add.
     * @param constructor A constructor function for the service.
     */
    template <typename T>
    void addService(std::function<void *(ServiceCollection *)> constructor)
    {
        constructorMap[new cobold::sys::Object<T>()] = constructor;
    }

    /**
     * @brief Add a service to the collection with a pointer to the service.
     *
     * The service can be later retrieved using the getService method.
     *
     * @tparam T The type of the service to add.
     * @param service A pointer to the service.
     */
    template <typename T>
    void addService(T *service)
    {
        services[new cobold::sys::Object<T>()] = [service]() -> void *
        { return service; };
    }

    void addExternalService(cobold::sys::BaseObject* typeWrapper, std::function<void *(ServiceCollection *)> constructor)
    {
        constructorMap[typeWrapper] = constructor;
    }

    void addExternalService(cobold::sys::BaseObject* typeWrapper, void* service)
    {
        services[typeWrapper] = [service]() -> void* { return service; };
    }

    /**
     * @brief Get a service from the collection.
     *
     * @tparam T The type of the service to retrieve.
     * @return A pointer to the retrieved service, or nullptr if not found.
     */
    template <typename T>
    T *getService()
    {
        cobold::sys::BaseObject *typeWrapper = new cobold::sys::Object<T>();
        std::string typeName = typeWrapper->getTypeName();

        logger->verbose("Looking for service: %s", typeWrapper->getTypeName().c_str());

        // Check in the services map
        // logger->verbose("Checking services map");
        auto it = services.begin();
        while (it != services.end())
        {
            //logger->verbose("Checking service: %s", it->first->GetTypeName().c_str());
            cobold::sys::BaseObject *wrapper = it->first;

            if (wrapper->getTypeName() == typeName)
            {
                // logger->verbose("Service found");
                void *servicePtr = it->second();
                delete typeWrapper; // Clean up typeWrapper
                return reinterpret_cast<T *>(servicePtr);
            }
            ++it;
        }

        // Check in the constructorMap
        // logger->verbose("Checking constructorMap");
        auto constructorIt = constructorMap.begin();
        while (constructorIt != constructorMap.end())
        {
            cobold::sys::BaseObject *wrapper = constructorIt->first;
            if (wrapper->getTypeName() == typeName)
            {
                logger->verbose("Service found, creating new instance");
                void *newService = constructorIt->second(this);
                services[typeWrapper] = [newService]() -> void *
                { return newService; };
                // delete typeWrapper; // Do not delete beacause we use it in the services map
                return reinterpret_cast<T *>(newService);
            }
            ++constructorIt;
        }

        logger->verbose("Service not found and no constructor available");
        logger->verbose(typeWrapper->getTypeName().c_str());
        delete typeWrapper; // Clean up typeWrapper
        
        return nullptr;
    }

    // Helper function to check if a type is derived from IHosedService
    template <typename Derived, typename Base>
    struct is_derived
    {
        static constexpr bool value = std::is_base_of<Base, Derived>::value;
    };

    

    /**
     * @brief Update a service in the collection using a new constructor.
     *
     * @tparam T The type of the service to update.
     */
    template <typename T>
    void updateService()
    {
        cobold::sys::BaseObject *typeWrapper = new cobold::sys::Object<T>();
        auto it = services.find(typeWrapper);
        if (it != services.end())
        {
            void *oldService = it->second();
            void *newService = constructorMap[cobold::sys::Object<T>()](this);
            delete oldService;
            it->second = [newService]() -> void *
            { return newService; };
        }
    }

    // ToDo fix bad design here (should be private)
    std::map<cobold::sys::BaseObject *, std::function<void *()>> services = {};
    std::map<cobold::sys::BaseObject *, std::function<void *(ServiceCollection *)>> constructorMap = {};
    private:
    cobold::Logger *logger;
};