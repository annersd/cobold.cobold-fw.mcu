#pragma once

#include <string>
#include <functional>
#include <vector>

class Event1
{
private:
    /* data */
    std::string name;
    void *data;

public:
    Event1(std::string name, void* data){
        this->name = name;
        this->data = data;
    };

    Event1(){
        this->name = "n/a";
        this->data = nullptr;
    };

    ~Event1(){};

    std::string getName()
    {
        return this->name;
    };

    void *getData()
    {
        return this->data;
    };

    void setName(std::string name)
    {
        this->name = name;
    };

    void setData(void *data)
    {
        this->data = data;
    };
};

class Event2
{
private:
    /* data */
    std::string name;
    void *data;

public:

    Event2(std::string name, void* data){
        this->name = name;
        this->data = data;
    };

    Event2(){
        this->name = "n/a";
        this->data = nullptr;
    };

    ~Event2(){};

    std::string getName()
    {
        return this->name;
    };

    template <typename T>
    T *getData()
    {
        return this->data;
    };

    void setName(std::string name)
    {
        this->name = name;
    };

    template <typename T>
    void setData(T *data)
    {
        this->data = data;
    };
};

class EventHandler
{
private:
    /* data */
    std::string eventName;
    std::function<void(Event2 *)> eventHandler;

public:
    EventHandler(std::string eventName, std::function<void(Event2 *)> eventHandler)
    {
        this->eventName = eventName;
        this->eventHandler = eventHandler;
    };
    ~EventHandler(){};

    std::string getEventName()
    {
        return this->eventName;
    };

    std::function<void(Event2 *)> getEventHandler()
    {
        return this->eventHandler;
    };
};


