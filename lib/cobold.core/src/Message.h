#pragma once

#include <cctype>
#include <Arduino.h>


typedef uintptr_t WPARAM;
typedef uintptr_t LPARAM;
typedef uint64_t Handle_t;

struct Message
{
    Handle_t senderId;
    Handle_t receiverId;
    uint64_t messageType;
    WPARAM wParam;
    LPARAM lParam;
};

