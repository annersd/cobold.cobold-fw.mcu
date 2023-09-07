#pragma once

#include <map>
#include <functional>
#include "Message.h"

class MessageBus
{

    // define message callback fuction type
    typedef void (*MessageCallback)(Message);

public:
    MessageBus()
    {
        xQueue1 = xQueueCreate(50, sizeof(Message));
    }
    void sendMessage(Message message)
    {
        xQueueSend(xQueue1, &message, 0);
    }
    void sendMessage(Handle_t senderId, Handle_t receiverId, uint64_t messageType, WPARAM wParam, LPARAM lParam)
    {
        Message message;
        message.senderId = senderId;
        message.receiverId = receiverId;
        message.messageType = messageType;
        message.wParam = wParam;
        message.lParam = lParam;

        xQueueSend(xQueue1, &message, 0);
    }

    void subscribe(Handle_t receicerId, void (*callback)(Message))
    {
        subscribers[receicerId] = callback;
    }
    void unsubscribe(Handle_t receicerId)
    {
        subscribers.erase(receicerId);
    }

    /**
     * @brief Pump messages from the queue and call the callback for the message receiver
     * @details This function is called from a task. It waits for a message to arrive and
     *          then calls the callback for the message receiver.
     */
    void pumpMessages()
    {
        Message message;
        // wait for a message to arrive
        while (xQueueReceive(xQueue1, &message, portMAX_DELAY) == pdTRUE)
        {
            // call the callback for the message receiver
            if (subscribers.find(message.receiverId) != subscribers.end())
            {
                subscribers[message.receiverId](message);
            }
        }
    }

    void createMessageProcessingTask()
    {
        BaseType_t xReturned;
        TaskHandle_t xHandle = NULL;

        xTaskCreate(
            [](void *param)
            {
                MessageBus *messageBus = static_cast<MessageBus *>(param);
                messageBus->pumpMessages(); // Call the member function
            },                              /* Function that implements the task. */
            "pumpMessagesTask",
            10000,
            this,
            1,
            NULL);
    }

private:
    QueueHandle_t xQueue1;
    std::map<Handle_t, MessageCallback> subscribers;
};