#pragma once

#include <Arduino.h>
#include <vector>
#include <functional>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include "IApplication.h"
#include "Logger.h"

#include "UUID.h"

class Scheduler
{
public:
    struct StateObject
    {
        void *state;
    };

    using ScheduleFunc = std::function<void(StateObject &)>;

    struct SchedulerItem
    {
        std::string name;
        UUID uuid;
        enum Type
        {
            // ToDo: Add immediate type (no delay) as a dispatcher equivalent?
            TIMEOUT,
            INTERVAL
        } type;
        union
        {
            uint32_t interval;
            uint32_t timeout;
        };
        uint32_t executionTime;
        ScheduleFunc scheduleFunc;
        StateObject stateObj;

        SchedulerItem(const std::string &n, Type t, uint32_t val, uint32_t execTime, const ScheduleFunc &tsk, const StateObject &state)
            : name(n), type(t), executionTime(execTime), scheduleFunc(tsk), stateObj(state)
        {
            if (type == TIMEOUT)
            {
                timeout = val;
            }
            else
            {
                interval = val;
            }

            uint32_t seed1 = random(999999999);
            uint32_t seed2 = random(999999999);

            uuid.seed(seed1, seed2);
            uuid.generate();
        }
    };

    Scheduler(cobold::IApplication *app)
    {
        this->app = app;
        logger = app->getServices()->getService<cobold::Logger>();

        // Create a mutex to protect access to the items vector
        itemsMutex = xSemaphoreCreateMutex();
    }

    ~Scheduler()
    {
        // Delete the mutex when the Scheduler is destroyed
        vSemaphoreDelete(itemsMutex);
    }

    // Schedule a scheduleFunc to run after a certain delay (in milliseconds)
    void schedule(uint32_t delayMs, const ScheduleFunc &scheduleFunc, const std::string &name, uint32_t timeout, const StateObject &state)
    {
        SchedulerItem item = {name, SchedulerItem::TIMEOUT, timeout, millis() + delayMs, scheduleFunc, state};
        addItem(item);
    }

    // Schedule a scheduleFunc to run at intervals (in milliseconds)
    void scheduleInterval(uint32_t intervalMs, const ScheduleFunc &scheduleFunc, const std::string &name, uint32_t interval, const StateObject &state)
    {
        SchedulerItem item = {name, SchedulerItem::INTERVAL, interval, millis() + intervalMs, scheduleFunc, state};
        addItem(item);
    }

    // Call this function in your main loop to execute scheduled tasks
    void run()
    {
        // logger->debug("Scheduler run");

        uint32_t currentMillis = millis();

        /*  Get a copy of the items
         *  This is done to avoid locking the mutex for too long
         *  If we would lock the mutex for the whole run, we would not be able to add new items
         *  while the scheduler is running
         *  This means also, that deleting an item will not take effect until the next run
         */
        auto copiedItems = getItemsCopy();

        for (auto it = copiedItems.begin(); it != copiedItems.end();)
        {
            if (currentMillis >= it->executionTime)
            {

                /* run the scheduleFunc
                *  We execute the function not in a scheduleFunc dispatcher.
                *  This means, that the scheduleFunc will run in the same context as the scheduler.
                *  By doing this we prevent flooding the scheduleFunc dispatcher with tasks and
                *  reduce the psoibility to exhaust the device resources.
                */
                it->scheduleFunc(it->stateObj);

                if (it->type == SchedulerItem::INTERVAL)
                {
                    it->executionTime = currentMillis + it->interval;
                }
                else
                {
                    // Delete the item from the original vector
                    deleteItemByID(it->uuid.toCharArray());
                }
                ++it;
            }
            else
            {
                ++it;
            }
        }
    }

    // Method to delete a scheduler item by name
    void deleteItemByName(const std::string &name)
    {
        // Take the mutex to protect access to the items vector
        xSemaphoreTake(itemsMutex, portMAX_DELAY);

        auto it = items.begin();
        while (it != items.end())
        {
            if (it->name == name)
            {
                it = items.erase(it);
            }
            else
            {
                ++it;
            }
        }

        // Give back the mutex
        xSemaphoreGive(itemsMutex);
    }

    // Method to delete a scheduler item by reference
    void deleteItemByReference(const SchedulerItem &item)
    {
        // Take the mutex to protect access to the items vector
        xSemaphoreTake(itemsMutex, portMAX_DELAY);

        auto it = items.begin();
        while (it != items.end())
        {
            if (&(*it) == &item)
            {
                Serial.println("Deleting item");
                it = items.erase(it);
            }
            else
            {
                ++it;
            }
        }

        // Give back the mutex
        xSemaphoreGive(itemsMutex);
    }

    // Method to delete a scheduler item by reference
    void deleteItemByID(char *uuid)
    {
        // Take the mutex to protect access to the items vector
        xSemaphoreTake(itemsMutex, portMAX_DELAY);

        auto it = items.begin();
        while (it != items.end())
        {
            if (strcmp(it->uuid.toCharArray(), uuid) == 0)
            {
                it = items.erase(it);
            }
            else
            {
                ++it;
            }
        }

        // Give back the mutex
        xSemaphoreGive(itemsMutex);
    }

private:
    std::vector<SchedulerItem> items;
    SemaphoreHandle_t itemsMutex;
    cobold::Logger *logger;
    cobold::IApplication *app;

    void addItem(const SchedulerItem &item)
    {
        logger->debug("Adding item %s", item.name.c_str());
        // Take the mutex to protect access to the items vector
        xSemaphoreTake(itemsMutex, portMAX_DELAY);
        items.push_back(item);
        // Give back the mutex
        xSemaphoreGive(itemsMutex);
    }

    // Method to get a copy of the available items
    std::vector<SchedulerItem> getItemsCopy()
    {
        // Take the mutex to protect access to the items vector
        xSemaphoreTake(itemsMutex, portMAX_DELAY);

        // Create a copy of the items
        std::vector<SchedulerItem> copyItems = items;

        // Give back the mutex
        xSemaphoreGive(itemsMutex);

        return copyItems;
    }
};
