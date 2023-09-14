#pragma once

#include <string>
#include <functional>
#include <vector>
#include "Object.h"

namespace cobold
{
    namespace sys
    {

        class Event
        {
        private:
            /* data */
            std::string source;
            std::string type;
            cobold::sys::BaseObject *data;

        public:
            Event(std::string source, std::string type, cobold::sys::BaseObject *data)
            {
                this->source = source;
                this->type = type;
                this->data = data;
            };

            ~Event(){};

            std::string getSource()
            {
                return this->source;
            };

            std::string getType()
            {
                return this->type;
            };

            cobold::sys::BaseObject *getData()
            {
                return this->data;
            };

            template <typename TEventData>
            static Event *create(std::string source, std::string type, TEventData *data)
            {
                return new Event(source, type, new cobold::sys::Object<TEventData>(data));
            };
        };

        class EventHandler
        {
        private:
            /* data */
            std::string source;
            std::string type;
            std::function<void(Event *)> eventHandler;

        public:
            EventHandler(std::string source, std::function<void(Event *)> eventHandler)
            {
                this->source = source;
                this->eventHandler = eventHandler;
            };

            ~EventHandler(){};

            std::string getSource()
            {
                return this->source;
            };

            std::string getType()
            {
                return this->type;
            };

            std::function<void(Event *)> getEventHandler()
            {
                return this->eventHandler;
            };

            template <typename TEventData>
            static EventHandler *create(std::string source, std::string type, std::function<void(TEventData *)> eventHandler)
            {
                return new EventHandler(source, [eventHandler](Event *event) -> void
                                        {
                    auto eventData = cobold::sys::unwrap<TEventData>(event->getData());
                    if (eventData != nullptr)
                    {
                        eventHandler(eventData);
                    }
                    else
                    {
                        eventHandler(nullptr);
                    } });
            };
        };

    } // namespace sys
} // namespace cobold