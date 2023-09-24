#pragma once
#include "Object.h"
#include "IApplication.h"
#include "Logger.h"
#include "Event.h"

namespace cobold::sys
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
            this->logger = app->getServices()->getService<cobold::Logger>();

            logger->debug("[EventDispatcher] - constructor");
            this->mutex = xSemaphoreCreateMutex();
        };
        ~EventDispatcher(){};

        void registerEventHandler(cobold::sys::EventHandler *eventHandler)
        {
            logger->debug("[EventDispatcher] - Registering event handler for event: %s", eventHandler->getSource().c_str());
            xSemaphoreTake(this->mutex, portMAX_DELAY);

            // register event handler
            this->eventHandlers.push_back(eventHandler);

            xSemaphoreGive(this->mutex);
        };

        void dispatch(Event *event)
        {
            std::vector<EventHandler *> localHandlers = this->getItemsCopy();

            logger->info("[EventDispatcher] - Dispatching event: [%s].[%s]", event->getSource().c_str(), event->getType().c_str());

            // lookup registered event handler

            for (auto &eventHandler : localHandlers)
            {
                // logger->debug("Checking event handler for event: %s", eventHandler->getSource().c_str());

                if (eventHandler->getSource() == event->getSource())
                {
                    auto eh = eventHandler->getEventHandler();
                    // logger->debug("Dispatching event: %s", event->getSource().c_str());

                    auto log = logger;
                    app->dispatch([eh, event, log]() -> void
                                  {
                                    try
                                    {
                                        eh(event);
                                    }
                                    catch(const std::exception& e)
                                    {
                                        log->error("[EventDispatcher] - Exception in event handler: %s", e.what());
                                    } });
                }
            }
        };

        // Method to get a copy of the available items
        std::vector<EventHandler *> getItemsCopy()
        {
            // Take the mutex to protect access to the items vector
            xSemaphoreTake(mutex, portMAX_DELAY);

            // Create a copy of the items
            std::vector<EventHandler *> copyItems = eventHandlers;
            // logger->debug("[EventDispatcher] - Copy %d Eventhandler(s) to immutable list", copyItems.size());

            // Give back the mutex
            xSemaphoreGive(mutex);

            return copyItems;
        }
    };

} // namespace cobold::sys