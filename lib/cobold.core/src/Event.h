#pragma once

#include <string>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/event_groups.h>
#include <freertos/semphr.h>
#include "IApplication.h"
#include "Logger.h"

class Event
{
private:
    /* data */
    std::string name;
    void *data;
    SemaphoreHandle_t mutex;

public:
    Event(std::string name, void* data){
        this->name = name;
        this->data = data;
        this->mutex = xSemaphoreCreateMutex();
    };

    Event(){
        this->name = "n/a";
        this->data = nullptr;
        this->mutex = xSemaphoreCreateMutex();
    };

    ~Event(){};

    std::string getName()
    {
        return this->name;
    };

    void *getData()
    {
        return this->data;
    };

    void setName(std::string name)
    {
        this->name = name;
    };

    void setData(void *data)
    {
        this->data = data;
    };
};

class EventHandler
{
private:
    /* data */
    std::string eventName;
    std::function<void(Event *)> eventHandler;

public:
    EventHandler(std::string eventName, std::function<void(Event *)> eventHandler)
    {
        this->eventName = eventName;
        this->eventHandler = eventHandler;
    };
    ~EventHandler(){};

    std::string getEventName()
    {
        return this->eventName;
    };

    std::function<void(Event *)> getEventHandler()
    {
        return this->eventHandler;
    };
};

class EventDispatcher
{
private:
    /* data */
    cobold::IApplication *app;
    std::vector<EventHandler> eventHandlers;
    SemaphoreHandle_t mutex;
    cobold::Logger *logger;

public:
    EventDispatcher(cobold::IApplication *app)
    {
        this->app = app;
        this->mutex = xSemaphoreCreateMutex();
        this->logger = app->getServices()->getService<cobold::Logger>();
    };
    ~EventDispatcher(){};

    void registerEventHandler(std::string eventName, std::function<void(Event *)> eventHandler)
    {
        logger->debug("Registering event handler for event: %s" , eventName);
        xSemaphoreTake(this->mutex, portMAX_DELAY);
        this->eventHandlers.push_back(EventHandler(eventName, eventHandler));
        xSemaphoreGive(this->mutex);
    };

    void dispatch(Event event)
    {

        // lookup registered event handler
        xSemaphoreTake(this->mutex, portMAX_DELAY);

        for (auto &eventHandler : this->eventHandlers)
        {
            if (eventHandler.getEventName() == event.getName())
            {
                auto eh = eventHandler.getEventHandler();
                logger->debug("Dispatching event: %s" , event.getName());
                app->dispatch([&eh, &event]() -> void
                              {
                                  eh(&event);
                              });
            }
        }

        xSemaphoreGive(this->mutex);
    };

    void dispatch(std::string eventName, void *data)
    {
        Event event;
        event.setName(eventName);
        event.setData(data);
        this->dispatch(event);
    };

    void dispatch(std::string eventName)
    {
        Event event;
        event.setName(eventName);
        this->dispatch(event);
    };
};
