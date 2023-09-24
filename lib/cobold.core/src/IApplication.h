#pragma once
#include "Object.h"
#include "ServiceCollection.h"
#include "IHostBuilder.h"
#include "IHost.h"

#include "Event.h"
namespace cobold
{
    namespace application
    {
        typedef std::function<void()> OnLoopCallback;
        typedef std::function<void(cobold::sys::Event*)> OnEventCallback;
        typedef std::function<void(const std::function<void(void)>& op)> OnDispatchCallback;
    } // namespace application

    class IApplication
    {
    public:
        virtual ~IApplication() {}

        /**
         * @brief Implement your preSetup logic here
         * 
         * @note This method is called before the setup() method
        */
        virtual void preSetup() = 0;

        /**
         * @brief Implement your setup logic here
         * 
         * @note This method is called after the preSetup() method
        */
        virtual void setup() = 0;

        /**
         * @brief The main loop of the application
        */
        virtual void loop() = 0;

        /**
         * @brief Run the application
        */
        virtual void run() = 0;

        virtual void dispatch(std::function<void()> function) = 0;
        virtual void raiseEvent(cobold::sys::Event* event) = 0;
        virtual void raiseEvent(std::string source, cobold::sys::EventArgs* eventArgs) = 0;

        virtual ServiceCollection *getServices() = 0;
        virtual cobold::hosting::IHost *getHost() = 0;
        virtual cobold::hosting::IHostBuilder *getHostBuilder() = 0;
        virtual cobold::configuration::IConfiguration *getAppConfiguration() = 0;

        virtual void onLoop(application::OnLoopCallback callback) = 0;
        virtual void onEvent(std::function<void(cobold::sys::Event*)> callback) = 0;
        virtual void onDispatch(application::OnDispatchCallback callback) = 0;
    };

    // cobold::IApplication *app = nullptr;
} // namespace cobold

