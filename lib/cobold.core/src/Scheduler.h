#include <Arduino.h>
#include <vector>
#include <functional>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

class Scheduler
{
public:
    struct StateObject
    {
        void *state;
    };

    using ScheduleTask = std::function<void(StateObject &)>;

    struct SchedulerItem
    {
        std::string name;
        enum Type
        {
            //ToDo: Add immediate type (no delay) as a dispatcher equivalent?
            TIMEOUT,
            INTERVAL
        } type;
        union
        {
            uint32_t interval;
            uint32_t timeout;
        };
        uint32_t executionTime;
        ScheduleTask task;
        StateObject stateObj;

        SchedulerItem(const std::string &n, Type t, uint32_t val, uint32_t execTime, const ScheduleTask &tsk, const StateObject &state)
            : name(n), type(t), executionTime(execTime), task(tsk), stateObj(state)
        {
            if (type == TIMEOUT)
            {
                timeout = val;
            }
            else
            {
                interval = val;
            }
        }
    };

    Scheduler()
    {
        // Create a mutex to protect access to the items vector
        itemsMutex = xSemaphoreCreateMutex();
    }

    ~Scheduler()
    {
        // Delete the mutex when the Scheduler is destroyed
        vSemaphoreDelete(itemsMutex);
    }

    // Schedule a task to run after a certain delay (in milliseconds)
    void schedule(uint32_t delayMs, const ScheduleTask &task, const std::string &name, uint32_t timeout, const StateObject &state)
    {
        SchedulerItem item = {name, SchedulerItem::TIMEOUT, timeout, millis() + delayMs, task, state};
        addItem(item);
    }

    // Schedule a task to run at intervals (in milliseconds)
    void scheduleInterval(uint32_t intervalMs, const ScheduleTask &task, const std::string &name, uint32_t interval, const StateObject &state)
    {
        SchedulerItem item = {name, SchedulerItem::INTERVAL, interval, millis() + intervalMs, task, state};
        addItem(item);
    }

    // Call this function in your main loop to execute scheduled tasks
    void run()
    {
        uint32_t currentMillis = millis();

        // Get a copy of the items
        // This is done to avoid locking the mutex for too long
        auto copiedItems = getItemsCopy();

        for (auto it = copiedItems.begin(); it != copiedItems.end();)
        {
            if (currentMillis >= it->executionTime)
            {
                
                // run the task
                it->task(it->stateObj);

                if (it->type == SchedulerItem::INTERVAL)
                {
                    it->executionTime = currentMillis + it->interval;
                }
                else
                {
                    // Delete the item from the original vector
                    deleteItemByReference(*it);
                }
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

    void addItem(const SchedulerItem &item)
    {
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
