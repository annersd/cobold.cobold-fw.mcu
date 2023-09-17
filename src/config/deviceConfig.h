#pragma once
#include "Arduino.h"

char deviceConfig[] PROGMEM = R"(
  {
    "cobold":
    {
      "mqtt":
      {
        "host": "192.168.0.69",
        "port": 1883,
        "clientid": "cobold"
      },
      "network": 
      { 
        "wifi":
        { 
            "ssid": "enter your ssid here", 
            "password": "enter your password here" 
        } 
      } 
    }
  })";
