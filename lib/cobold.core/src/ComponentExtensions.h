#pragma once

#include "IComponent.h"
#include "Logger.h"
#include <vector>
#include "ServiceCollection.h"


namespace cobold
{
    namespace components
    {

        class ComponentExtensions
        {
        private:
            /* data */
        public:
            /**
             * @brief Get a list of services that inherit from the specified interface.
             *
             * @tparam TInterface The interface type to check for.
             * @return A vector of services that inherit from the specified interface.
             */
            static std::vector<IComponent *> GetComponents(ServiceCollection *serviceCollection)
            {
                std::vector<IComponent *> componentsList;
                SerialLogger *logger = serviceCollection->getService<SerialLogger>();

                // Check the existing services map
                for (auto it = serviceCollection->services.begin(); it != serviceCollection->services.end(); ++it)
                {
                    cobold::sys::BaseObject *typeWrapper = it->first;

                    if (cobold::components::ComponentExtensions::IsComponent(typeWrapper))
                    {
                        logger->verbose("Component found");
                        void *componentPtr = it->second();
                        IComponent *component = reinterpret_cast<IComponent *>(componentPtr);
                        if (component)
                        {
                            componentsList.push_back(component);
                        }
                    }
                }

                // Check the constructorMap for new services
                for (auto constructorIt = serviceCollection->constructorMap.begin(); constructorIt != serviceCollection->constructorMap.end(); ++constructorIt)
                {
                    cobold::sys::BaseObject *typeWrapper = constructorIt->first;

                    // Compare wrapped type with TInterface using is_base_of and typeid
                    if (cobold::components::ComponentExtensions::IsComponent(typeWrapper))
                    {
                        // if not already in the services map
                        bool isAlreadyInServicesMap = false;
                        for (auto it = serviceCollection->services.begin(); it != serviceCollection->services.end(); ++it)
                        {
                            cobold::sys::BaseObject *srvTypeWrapper = it->first;

                            if (srvTypeWrapper->getTypeName() == typeWrapper->getTypeName())
                            {
                                isAlreadyInServicesMap = true;
                                break;
                            }
                        }

                        if (isAlreadyInServicesMap)
                        {
                            continue;
                        }
                        else
                        {
                            logger->verbose("Component found, creating a new instance");
                            void *newComponent = constructorIt->second(serviceCollection);
                            serviceCollection->services[typeWrapper] = [newComponent]() -> void *
                            { return newComponent; };

                            IComponent *component = reinterpret_cast<IComponent *>(newComponent);
                            if (component)
                            {
                                componentsList.push_back(component);
                            }
                        }
                    }
                }

                return componentsList;
            }

            template <typename T>
            static void AddComponent(ServiceCollection *serviceCollection, std::function<void *(ServiceCollection *)> constructor)
            {
                cobold::sys::BaseObject *typeWrapper = new cobold::sys::Object<T>();

                typeWrapper->setProperty("cobold::components::IComponent", "true");
                serviceCollection->addExternalService(typeWrapper, constructor);
            }

            static bool IsComponent(cobold::sys::BaseObject *typeWrapper)
            {
                return typeWrapper->getProperty("cobold::components::IComponent") == "true";
            }
        };

    } // namespace components
} // namespace cobold
