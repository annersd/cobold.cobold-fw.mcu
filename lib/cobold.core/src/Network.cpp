#include "Network.h"


// #include "ServiceCollection.h"
#include "IApplication.h"
#include "Logger.h"

Network::Network(String ssid, String password)
    {
        this->ssid = ssid;
        this->password = password;
    }