#pragma once

#include <string>
#include <functional>
#include <vector>
#include "Object.h"

namespace cobold::sys
{

    /**
     * @brief Base class for event arguments
     * @details This class is used to pass data to event handlers
     * The data is serialized to JSON and deserialized from JSON if they are passed 
     * out of the local system 
    */
    class EventArgs
    {
    public:
        EventArgs() = default;
        ~EventArgs() = default;

        /**
         * @brief Get the type of the event arguments
         * @details This method is used to identify the type of the event globally
         * @return The type of the event arguments
        */
        virtual const char* getType() const { return "net.cobold.sys.event"; }

        /**
         * @brief Serialize to JSON
         * @details This method is used to serialize the event arguments to JSON
         * @param pretty If the JSON string should be pretty printed (default: false)
         * @return The JSON string 
        */
        virtual std::string toJson(bool pretty = false) = 0;

        /**
         * @brief Deserialize from JSON
         * @details This method is used to deserialize the event arguments from JSON
         * @param json The JSON string
         * @return If the deserialization was successful
        */
        virtual bool fromJson(std::string json) = 0;
    };

    class Event
    {
    protected:
        /* data */
        std::string source;
        std::string type;
        cobold::sys::BaseObject *data;

    public:
        Event(){};
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

} // namespace cobold::sys