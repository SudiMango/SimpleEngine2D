#pragma once

#include <map>
#include <vector>
#include <functional>
#include <typeindex>
#include <algorithm>

namespace simpleengine2d::core {

class EventBus {

public:
    using EventCallback = std::function<void(void*)>;

    static EventBus &getInstance() {
        static EventBus instance;
        return instance;
    };

    template<typename T>
    void subscribe(void *owner, EventCallback callback) {
        eventListeners[typeid(T)].push_back({owner, callback});
    }

    void unsubscribe(void *owner) {
        for (auto &pair : eventListeners) {
            auto &listeners = pair.second;
            listeners.erase(std::remove_if(listeners.begin(), listeners.end(),
                [&](const std::pair<void*, EventCallback> &listener) {
                    return listener.first == owner;
                }),
            listeners.end());
        }
    }

    template<typename T>
    void publish(T *event) {
        auto it = eventListeners.find(typeid(T));
        if (it != eventListeners.end()) {
            for (auto const &[owner, callback] : it->second) {
                callback(event);
            }
        }
    }

private:
    EventBus() = default;
    ~EventBus() = default;
    
    EventBus(const EventBus&) = delete;
    EventBus& operator=(const EventBus&) = delete;

    std::map<std::type_index, std::vector<std::pair<void*, EventCallback>>> eventListeners;

};

}