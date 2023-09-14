#pragma once

#include "IApplication.h"
#include "Logger.h"
#include "Event.h"
#include "Object.h" 

class EventDispatcher
{
private:
    /* data */
    cobold::IApplication *app;
    std::vector<EventHandler*> eventHandlers;
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

    template <typename T>
    void registerEventHandler(std::string eventName, std::function<void(T *)> eventHandler)
    {
        logger->debug("Registering event handler for event: %s", eventName);
        xSemaphoreTake(this->mutex, portMAX_DELAY);

        // register event handler
        this->eventHandlers.push_back( new EventHandler(eventName, [eventHandler] (Event2* event) -> void {
            Serial.println("Calling event handler");
            T* data = event->getData();
            Serial.println("reinterpret_cast");
            auto object = reinterpret_cast<T*>(data);
            Serial.println("Calling the inner event handler");
            try
            {
                eventHandler(object);
            }
            catch(const std::exception& e)
            {
                Serial.println("Exception in event handler");
            }
            
            
        }));

        xSemaphoreGive(this->mutex);
    };

    void dispatch(Event2 event)
    {
        std::vector<EventHandler*> localHandlers = this->getItemsCopy();
        Serial.println("EventDispatcher::dispatch-loop");
   
        // lookup registered event handler
        // xSemaphoreTake(this->mutex, portMAX_DELAY);

        for (auto &eventHandler : localHandlers)
        {
            Serial.println(eventHandler->getEventName().c_str());
            if (eventHandler->getEventName() == event.getName())
            {
                auto eh = eventHandler->getEventHandler();
                logger->debug("Dispatching event: %s", event.getName());
                app->dispatch([&eh, &event]() -> void
                              { 
                                Serial.println("Calling event handler");
                                try
                                {
                                    //eh(&event); 
                                }
                                catch(const std::exception& e)
                                {
                                    Serial.println("Exception in event handler");
                                }
                            });
            }
        }

        // xSemaphoreGive(this->mutex);
    };

    template <typename T>
    void dispatch(std::string eventName, T *data)
    {
        Serial.println("Dispatching event");
        Event2 event(eventName, new cobold::sys::Object<T>(data));
        this->dispatch(event);
    };

    template <typename T>
    void dispatch(std::string eventName)
    {
        Event2 event(eventName, nullptr);
        this->dispatch(event);
    };

    // Method to get a copy of the available items
    std::vector<EventHandler*> getItemsCopy()
    {
        // Take the mutex to protect access to the items vector
        xSemaphoreTake(mutex, portMAX_DELAY);

        // Create a copy of the items
        std::vector<EventHandler*> copyItems = eventHandlers;
        Serial.println("EventDispatcher::getItemsCopy");
        Serial.println(copyItems.size());

        // Give back the mutex
        xSemaphoreGive(mutex);

        return copyItems;
    }
};
