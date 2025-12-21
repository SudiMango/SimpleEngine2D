#pragma once

#include <map>
#include <vector>
#include <functional>
#include <typeindex>

namespace simpleengine2d::core {

class EventBus {

public:
    using EventCallback = std::function<void(void*)>;

    static EventBus &getInstance() {
        static EventBus instance;
        return instance;
    };

    template<typename T>
    void subscribe(EventCallback callback) {
        eventListeners[typeid(T)].push_back(callback);
    }

    template<typename T>
    void publish(T *event) {
        auto it = eventListeners.find(typeid(T));
        if (it != eventListeners.end()) {
            for (auto callback : it->second) {
                callback(event);
            }
        }
    }

private:
    EventBus() = default;
    ~EventBus() = default;
    
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

    std::map<std::type_index, std::vector<EventCallback>> eventListeners;

};

}