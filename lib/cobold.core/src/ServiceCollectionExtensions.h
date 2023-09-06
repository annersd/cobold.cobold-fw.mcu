#pragma once

#include "cobold.hpp"

namespace cobold
{
    namespace services
    {

        template <typename TServiceType>
        class ServiceCollectionExtensions
        {
        private:
            /* data */
        public:
            /**
             * @brief Get a list of services that inherit from the specified interface.
             *
             * @return A vector of services that inherit from the specified interface.
             */
            static std::vector<TServiceType *> GetServices(ServiceCollection *serviceCollection)
            {
                std::vector<TServiceType *> servicesList;
                ILogger *logger = serviceCollection->getService<ILogger>();

                // Check the existing services map
                for (auto it = serviceCollection->services.begin(); it != serviceCollection->services.end(); ++it)
                {
                    ITypeWrapper *typeWrapper = it->first;

                    if (cobold::services::ServiceCollectionExtensions<TServiceType>::IsService(typeWrapper))
                    {
                        logger->verbose("Service found");
                        void *servicePtr = it->second();
                        TServiceType *service = reinterpret_cast<TServiceType *>(servicePtr);
                        if (service)
                        {
                            servicesList.push_back(service);
                        }
                    }
                }

                // Check the constructorMap for new services
                for (auto constructorIt = serviceCollection->constructorMap.begin(); constructorIt != serviceCollection->constructorMap.end(); ++constructorIt)
                {
                    ITypeWrapper *typeWrapper = constructorIt->first;

                    // Compare wrapped type with TServiceType using is_base_of and typeid
                    if (cobold::services::ServiceCollectionExtensions<TServiceType>::IsService(typeWrapper))
                    {
                        // if not already in the services map
                        bool isAlreadyInServicesMap = false;
                        for (auto it = serviceCollection->services.begin(); it != serviceCollection->services.end(); ++it)
                        {
                            ITypeWrapper *srvTypeWrapper = it->first;

                            if (srvTypeWrapper->GetName() == typeWrapper->GetName())
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
                            logger->verbose("Service found, creating a new instance");
                            void *newService = constructorIt->second(serviceCollection);
                            serviceCollection->services[typeWrapper] = [newService]() -> void *
                            { return newService; };

                            TServiceType *service = reinterpret_cast<TServiceType *>(newService);
                            if (service)
                            {
                                servicesList.push_back(service);
                            }
                        }
                    }
                }

                return servicesList;
            }

            template <typename T>
            static void AddService(ServiceCollection *serviceCollection, std::function<void *(ServiceCollection *)> constructor)
            {
                ITypeWrapper *typeWrapper = new TypeWrapper<T>();

                typeWrapper->SetProperty(GetServiceTypeProperty(), "true");
                serviceCollection->addExternalService(typeWrapper, constructor);
            }

            static bool IsService(ITypeWrapper *typeWrapper)
            {
                return typeWrapper->GetProperty(GetServiceTypeProperty()) == "true";
            }

            static std::string GetServiceTypeProperty()
            {
                std::string serviceTypeProperty = typeid(TServiceType).name();
                return serviceTypeProperty;
            }
        };
    } // namespace services
} // namespace cobold
