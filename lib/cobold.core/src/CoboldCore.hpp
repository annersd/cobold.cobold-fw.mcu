#pragma once

#define COBOLD_CORE_VERSION_MAJOR 0
#define COBOLD_CORE_VERSION_MINOR 1
#define COBOLD_CORE_VERSION_PATCH 0

#include "Logger.h"
#include "IApplication.h"
#include "ServiceCollection.h"
#include "ServiceCollectionExtensions.h"

// Include Hosting
#include "IHost.h"
#include "IHostBuilder.h"
#include "IHostedService.h"
#include "Host.h"
#include "HostBuilder.h"
#include "HostingExtensions.h"

// Include Configuration
#include "IConfiguration.h"
#include "Configuration.h"

#include "Application.h"

namespace cobold
{
    static const cobold::IApplication *app = new cobold::Application(new cobold::hosting::HostBuilder());
} // namespace cobold
