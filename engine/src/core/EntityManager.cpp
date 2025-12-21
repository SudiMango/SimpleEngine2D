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
        // TODO: clean up
    }
}

std::vector<EntityId> EntityManager::getAllEntities() {
    return entities;
}

}
