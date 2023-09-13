#pragma once

#include "IApplication.h"
#include "Logger.h"
#include "Dispatcher.h"
#include "Event.h"


class EventDispatcher
{
private:
    /* data */
    cobold::IApplication *app;
    //std::vector<BaseEventHandler> eventHandlers;
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

    template <typename T>
    void registerEventHandler(std::string eventName, std::function<void(T *)> eventHandler)
    {
        logger->debug("Registering event handler for event: %s", eventName);
        xSemaphoreTake(this->mutex, portMAX_DELAY);
        // this->eventHandlers.push_back(EventHandler<T>(eventName, eventHandler));
        xSemaphoreGive(this->mutex);
    };

    void dispatch(BaseEvent event)
    {

        // lookup registered event handler
        xSemaphoreTake(this->mutex, portMAX_DELAY);

        // for (auto &eventHandler : this->eventHandlers)
        // {
        //     if (eventHandler.getEventName() == event.getName())
        //     {
        //         auto eh = eventHandler.getEventHandler();
        //         logger->debug("Dispatching event: %s", event.getName());
        //         app->dispatch([&eh, &event]() -> void
        //                       { eh(&event); });
        //     }
        // }

        xSemaphoreGive(this->mutex);
    };

    template <typename T>
    void dispatch(std::string eventName, T *data)
    {
        BaseEvent event(eventName, new cobold::sys::Object<T>(data));
        this->dispatch(event);
    };

    template <typename T>
    void dispatch(std::string eventName)
    {
        BaseEvent event(eventName, nullptr);
        this->dispatch(event);
    };
};
