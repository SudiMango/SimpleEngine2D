#include "SimpleEngine2D/core/EntityManager.hpp"

namespace simpleengine2d::core {

EntityManager &EntityManager::getInstance() {
    static EntityManager instance;
    return instance;
}

/**
 * Entity stuff
 */

EntityId EntityManager::createEntity() {
    EntityId id = nextId++;
    entities.push_back(id);
    entityToComponents[id];
    return id;
}

void EntityManager::deleteEntity(EntityId id) {
    auto it = entityToComponents.find(id);
    if (it != entityToComponents.end()) {
        for (auto &cdata : it->second) {
            cdata.deleter(cdata.component);
        }
        entityToComponents.erase(it);
    }

    auto e_it = std::find(entities.begin(), entities.end(), id);
    if (e_it != entities.end()) {
        entities.erase(e_it);
    }
}

std::vector<EntityId> EntityManager::getAllEntities() {
    return entities;
}

}
