#pragma once

#include <map>
#include <typeindex>
#include <vector>
#include <algorithm>
#include <cstdint>

namespace simpleengine2d::core {

using EntityId = uint32_t;

struct ComponentData {
    std::type_index type;
    void *component;
    void (*deleter)(void*);
};

class EntityManager {

public:
    static EntityManager &getInstance();

    EntityId createEntity();
    void deleteEntity(EntityId id);
    std::vector<EntityId> getAllEntities();
    void reset() {
        entities.clear();
        entityToComponents.clear();
        nextId = 0;
    }

    template<typename T>
    void addComponent(EntityId id, T *component) {
        ComponentData data = {
            typeid(T),
            static_cast<void*>(component),
            [](void* p) { delete static_cast<T*>(p); }
        };

        entityToComponents[id].push_back(data);
    }

    template<typename T>
    void removeComponent(EntityId id) {
        auto it = entityToComponents.find(id);
        if (it != entityToComponents.end()) {
            auto &components = it->second;
            for (auto cit = components.begin(); cit != components.end(); ++cit) {
                if (cit->type == typeid(T)) {
                    cit->deleter(cit->component);
                    components.erase(cit);
                    break;
                }
            }
        }
    }

    template<typename T>
    bool hasComponent(EntityId id) {
        auto it = entityToComponents.find(id);
        if (it != entityToComponents.end()) {
            std::vector<ComponentData> components = entityToComponents[id];
            for (int i = 0; i < components.size(); i++) {
                if (components.at(i).type == typeid(T)) {
                    return true;
                }
            }
        }

        return false;
    }

    template<typename T>
    T *getComponent(EntityId id) {
        auto it = entityToComponents.find(id);
        if (it != entityToComponents.end()) {
            std::vector<ComponentData> components = entityToComponents[id];
            for (int i = 0; i < components.size(); i++) {
                if (components.at(i).type == typeid(T)) {
                    return static_cast<T*>(components.at(i).component);
                }
            }
        }

        return nullptr;
    }

private:
    EntityManager() = default;
    ~EntityManager() = default;

    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;

    EntityId nextId = 0;
    std::vector<EntityId> entities;
    std::map<EntityId, std::vector<ComponentData>> entityToComponents;

};

}