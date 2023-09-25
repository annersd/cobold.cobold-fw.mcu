#pragma once

#include "IApplication.h"
#include "IConfiguration.h"
#include "Configuration.h"

#include "MqttAppServices.h"
#include "NetworkAppServices.h"
#include "SchedulerAppServices.h"
#include "EventAppServices.h"
#include "TaskAppServices.h"
#include "NodeAppServices.h"
#include "ComponentAppServices.h"
#include "LoggerAppServices.h"

namespace cobold::application
{
    void deaultConfigure(IApplication *app, char *deviceConfig, char *secretsConfig)
    {
        // configure the application
        app->getAppConfiguration()->update(
            cobold::configuration::Configuration::fromJson(deviceConfig));
        app->getAppConfiguration()->update(
            cobold::configuration::Configuration::fromJson(secretsConfig));
    }

    void defaultPreSetup(IApplication *app)
    {
        app->preSetup();

        cobold::services::AddTaskService(app);
        cobold::services::AddEventService(app);
        cobold::services::AddSchedulerService(app);
        cobold::services::AddLogService(app);

        cobold::services::AddNetworkService(app);
        cobold::services::AddMqttClientService(app);

        cobold::services::AddNodeService(app);
        cobold::services::AddComponentService(app);
    }

    void defaultSetup(IApplication *app)
    {
        app->setup();
        
        cobold::services::UseLogService(app);
        cobold::services::UseTaskService(app);
        cobold::services::UseEventService(app);
        cobold::services::UseSchedulerService(app);

        cobold::services::UseNetworkService(app);
        cobold::services::UseMqttClientService(app);

        cobold::services::UseNodeService(app);
        cobold::services::UseComponentService(app);
    }

}