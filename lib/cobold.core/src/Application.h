#pragma once


#include "Object.h"
#include "IApplication.h"
#include "ServiceCollection.h"
#include "IHost.h"
#include "IHostBuilder.h"
#include "IConfiguration.h"
#include "Logger.h"
#include "Event.h"

#include "Scheduler.h"

namespace cobold
{

    class Application : public cobold::IApplication
    {
    public:
        Application(cobold::hosting::IHostBuilder *hostBuilder);
        ~Application();

        void preSetup() override;
        void setup() override;
        void loop() override;
        void run() override;
        void dispatch(std::function<void()> function) override;
        void raiseEvent(cobold::sys::Event* event) override;
        void raiseEvent(std::string source, cobold::sys::EventArgs* eventArgs) override;
        void onLoop(application::OnLoopCallback callback) override;
        void onEvent(std::function<void(cobold::sys::Event*)> callback) override;
        void onDispatch(application::OnDispatchCallback callback) override;

        ServiceCollection *getServices() override;
        cobold::hosting::IHost *getHost() override;
        cobold::hosting::IHostBuilder *getHostBuilder() override;
        cobold::configuration::IConfiguration *getAppConfiguration() override;
        Scheduler *scheduler;

    private:
        ServiceCollection *services;
        cobold::hosting::IHost *host;
        cobold::hosting::IHostBuilder *hostBuilder;
        cobold::configuration::IConfiguration *configuration;
        Logger *logger;
        std::vector<application::OnLoopCallback> loopCallbacks;
        std::vector<std::function<void(cobold::sys::Event*)>> eventCallbacks;
        std::vector<std::function<void(const std::function<void(void)>& op)>> dispatchCallbacks;
    };

    
} // namespace cobold
