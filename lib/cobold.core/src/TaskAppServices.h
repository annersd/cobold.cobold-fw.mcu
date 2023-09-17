#pragma once    

#include "IApplication.h"
#include "TaskDispatcher.h"

namespace cobold::services
{
    void AddTaskService(cobold::IApplication *app)
    {
        app->getHostBuilder()->configureServices([app](ServiceCollection *services) -> void
                                                 {
            // Add  service
            services->addService<dispatch_queue>([app](ServiceCollection *services) -> void *
                { return new dispatch_queue("main", 1, 4096*4); });
             });
    }

    void UseTaskService(cobold::IApplication *app)
    {
        auto logger = app->getServices()->getService<cobold::Logger>();
        logger->info("Setup Task");

        auto task = app->getServices()->getService<dispatch_queue>();
        app->onDispatch([task, logger](std::function<void()> func) -> void
                     { 
                        logger->verbose("[Task] - Dispatching task");
                        task->dispatch(func); 
                        });
    }


}