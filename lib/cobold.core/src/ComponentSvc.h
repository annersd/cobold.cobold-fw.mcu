#pragma once

#include "IApplication.h"
#include "IComponent.h"
#include "Logger.h"
#include <map>
#include <vector>
#include <regex>
#include <string>

namespace cobold::components
{
    class ComponentSvc
    {
    private:
        cobold::IApplication *app;
        std::vector<IComponent *> components;
        cobold::Logger *logger;

    public:

        ComponentSvc(cobold::IApplication *app) : app(app) {
            logger = app->getServices()->getService<cobold::Logger>();
            logger->info("Create Component Service");
            components = std::vector<IComponent *>();
        }

        void setup()
        {
            logger->info("Setup");

           
        }

        void initialize()
        {
            logger->info("Initialize Components");

            for (auto component : components)
            {
                component->initialize();
            }
        }

        void update()
        {
            logger->info("Update Components");

            for (auto component : components)
            {
                component->update();
            }

            logger->info("Update Components Done");
        }

        void configure()
        {
            logger->info("Configure Components");

            for (auto component : components)
            {
                component->configure();
            }
        }

        void addComponent(IComponent *component)
        {
            logger->info("Add Component: %s", component->getName().c_str());

            components.push_back(component);
        }


        IComponent *getComponent(std::string identifier)
        {
            // return the component with the given id
            for (auto component : components)
            {
                if (component->getId() == identifier)
                {
                    return component;
                }
            }

            return nullptr;
        }

        void removeComponent(std::string id)
        {
            logger->info("Remove Component: %s", id.c_str() );

            // remove the component with the given id
            for (auto component : components)
            {
                if (component->getId() == id)
                {
                    // components.erase(component);
                }
            }
        }

        std::vector<IComponent *> findComponentsById(std::string idRegEx)
        {
            std::vector<IComponent *> result;

            std::regex idRegex(idRegEx);

            for (auto component : components)
            {
                if (std::regex_match(component->getId(), idRegex))
                {
                    result.push_back(component);
                }
            }

            return result;
        }
    };
}