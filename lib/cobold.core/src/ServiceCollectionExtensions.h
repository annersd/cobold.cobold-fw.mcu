#pragma once

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
                SerialLogger *logger = serviceCollection->getService<SerialLogger>();

                // Check the existing services map
                for (auto it = serviceCollection->services.begin(); it != serviceCollection->services.end(); ++it)
                {
                    cobold::sys::BaseObject *typeWrapper = it->first;

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
                    cobold::sys::BaseObject *typeWrapper = constructorIt->first;

                    // Compare wrapped type with TServiceType using is_base_of and typeid
                    if (cobold::services::ServiceCollectionExtensions<TServiceType>::IsService(typeWrapper))
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
                cobold::sys::BaseObject *typeWrapper = new cobold::sys::Object<T>();

                typeWrapper->setProperty(GetServiceTypeProperty(), "true");
                serviceCollection->addExternalService(typeWrapper, constructor);
            }

            static bool IsService(cobold::sys::BaseObject *typeWrapper)
            {
                return typeWrapper->getProperty(GetServiceTypeProperty()) == "true";
            }

            static std::string GetServiceTypeProperty()
            {
                std::string serviceTypeProperty = typeid(TServiceType).name();
                return serviceTypeProperty;
            }
        };
    } // namespace services
} // namespace cobold
