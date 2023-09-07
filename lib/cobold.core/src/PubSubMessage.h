#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <functional>

class Message {
public:
    Message(const std::string& topic)
        : topic(topic) {}

    const std::string& getTopic() const {
        return topic;
    }

    const std::string& getResponseTopic() const {
        return responseTopic;
    }


private:
    std::string topic;
    std::string responseTopic;
    
    std::string contentType;
    void* content;
    
};
