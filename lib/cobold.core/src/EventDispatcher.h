#pragma once
#include "Object.h"
#include "IApplication.h"
#include "Logger.h"
#include "Event.h"


namespace cobold
{
    namespace sys
    {

        class EventDispatcher
        {
        private:
            /* data */
            cobold::IApplication *app;
            std::vector<EventHandler *> eventHandlers;
            SemaphoreHandle_t mutex;
            cobold::Logger *logger;

        public:
            EventDispatcher(cobold::IApplication *app)
            {
                this->app = app;
                this->mutex = xSemaphoreCreateMutex();
                Serial.println("EventDispatcher constructor");
                this->logger = app->getServices()->getService<cobold::Logger>();
            };
            ~EventDispatcher(){};


            void registerEventHandler(cobold::sys::EventHandler *eventHandler)
            {
                logger->debug("Registering event handler for event: %s", eventHandler->getSource().c_str());
                xSemaphoreTake(this->mutex, portMAX_DELAY);

                // register event handler
                this->eventHandlers.push_back(eventHandler);

                xSemaphoreGive(this->mutex);
            };

            void dispatch(Event *event)
            {
                std::vector<EventHandler *> localHandlers = this->getItemsCopy();
                
                logger->debug("Dispatching event: %s", event->getSource().c_str());

                // lookup registered event handler
                // xSemaphoreTake(this->mutex, portMAX_DELAY);

                for (auto &eventHandler : localHandlers)
                {
                    //logger->debug("Checking event handler for event: %s", eventHandler->getSource().c_str());

                    if (eventHandler->getSource() == event->getSource())
                    {
                        auto eh = eventHandler->getEventHandler();
                        //logger->debug("Dispatching event: %s", event->getSource().c_str());

                        app->dispatch([eh, event]() -> void
                                      {
                                        // Serial.println("Calling event handler");
                                        try
                                        {
                                            eh(event);
                                            //eh(&event);
                                        }
                                        catch(const std::exception& e)
                                        {
                                            Serial.println("Exception in event handler");
                                        }
                                    });

                        // app->getServices()->getService<Scheduler>()->schedule(0,
                        // [eh, event](const Scheduler::StateObject &state) -> void
                        //     {
                        //         Serial.println("Calling event handler");
                        //                 try
                        //                 {
                        //                     eh(event);
                        //                     //eh(&event);
                        //                 }
                        //                 catch(const std::exception& e)
                        //                 {
                        //                     Serial.println("Exception in event handler");
                        //                 } },
                        //     "", 5000, Scheduler::StateObject());
                    }
                }

                // xSemaphoreGive(this->mutex);
            };

            // template <typename T>
            // void dispatch(std::string eventName, T *data)
            // {
            //     Serial.println("Dispatching event");
            //     // Event event(eventName, new cobold::sys::Object<T>(data));
            //     // this->dispatch(event);
            // };

            // template <typename T>
            // void dispatch(std::string eventName)
            // {
            //     Event event(eventName, nullptr);
            //     this->dispatch(event);
            // };

            // Method to get a copy of the available items
            std::vector<EventHandler *> getItemsCopy()
            {
                // Take the mutex to protect access to the items vector
                xSemaphoreTake(mutex, portMAX_DELAY);

                // Create a copy of the items
                std::vector<EventHandler *> copyItems = eventHandlers;
                Serial.println("EventDispatcher::getItemsCopy");
                Serial.println(copyItems.size());

                // Give back the mutex
                xSemaphoreGive(mutex);

                return copyItems;
            }
        };

    } // namespace sys
} // namespace cobold