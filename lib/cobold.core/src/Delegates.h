#pragma once

#include <functional>
#include <vector>
#include "freertos/semphr.h"

namespace cobold::sys
{
    /**
     * @brief A delegate is a function pointer with a context.
     *        It can be used to register multiple functions to a single event.
     * @tparam TResult The return type of the delegate.
     * @tparam TArgs The argument types of the delegate.
     */
    // Default template with void return type
    template <typename TResult, typename... TArgs>
    class FunctionDelegate
    {
    private:
        using TFunction = std::function<TResult(TArgs...)>;

        std::vector<TFunction> _functions;
        SemaphoreHandle_t mutex;

    public:
        FunctionDelegate()
        {
            mutex = xSemaphoreCreateMutex();
        }

        void add(TFunction function)
        {
            xSemaphoreTake(mutex, portMAX_DELAY);
            _functions.push_back(function);
            xSemaphoreGive(mutex);
        }

        void remove(TFunction function)
        {
            xSemaphoreTake(mutex, portMAX_DELAY);
            _functions.erase(std::remove(_functions.begin(), _functions.end(), function), _functions.end());
            xSemaphoreGive(mutex);
        }

        void clear()
        {
            xSemaphoreTake(mutex, portMAX_DELAY);
            _functions.clear();
            xSemaphoreGive(mutex);
        }

        std::vector<TResult> invoke(TArgs... args)
        {
            std::vector<TResult> results;
            auto functions = _functions;

            for (auto &function : functions)
            {
                results.push_back(function(args...));
            }

            return results;
        }

        std::vector<TFunction> getInvocationList()
        {
            xSemaphoreTake(mutex, portMAX_DELAY);
            auto functions = _functions;
            xSemaphoreGive(mutex);

            return functions;
        }
    };

    /**
     * @brief A delegate is a function pointer with a context.
     *        It can be used to register multiple functions to a single event.
     * @tparam TResult The return type of the delegate.
     * @tparam TArgs The argument types of the delegate.
     */
    // Default template with void return type
    template <typename... TArgs>
    class ActionDelegate
    {
    private:
        using TFunction = std::function<void(TArgs...)>;

        std::vector<TFunction> _functions;
        SemaphoreHandle_t mutex;

    public:
        ActionDelegate()
        {
            mutex = xSemaphoreCreateMutex();
        }

        void add(TFunction function)
        {
            xSemaphoreTake(mutex, portMAX_DELAY);
            _functions.push_back(function);
            xSemaphoreGive(mutex);
        }

        void remove(TFunction function)
        {
            xSemaphoreTake(mutex, portMAX_DELAY);
            _functions.erase(std::remove(_functions.begin(), _functions.end(), function), _functions.end());
            xSemaphoreGive(mutex);
        }

        void clear()
        {
            xSemaphoreTake(mutex, portMAX_DELAY);
            _functions.clear();
            xSemaphoreGive(mutex);
        }

        void invoke(TArgs... args)
        {
            auto functions = _functions;

            for (auto &function : functions)
            {
                function(args...);
            }

        }

        std::vector<TFunction> getInvocationList()
        {
            xSemaphoreTake(mutex, portMAX_DELAY);
            auto functions = _functions;
            xSemaphoreGive(mutex);

            return functions;
        }
    };

    
} // namespace cobold::sys
