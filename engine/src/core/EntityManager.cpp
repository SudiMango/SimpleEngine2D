#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/components/Weld.hpp"

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
    if (it == entityToComponents.end()) {
        return;
    }

    // Collect welded children IDs before deleting the parent
    std::vector<EntityId> childrenToDelete;
    if (hasComponent<simpleengine2d::components::WeldComponent>(id)) {
        auto* weldComp = getComponent<simpleengine2d::components::WeldComponent>(id);
        if (weldComp) {
            for (const auto& weld : weldComp->welds) {
                childrenToDelete.push_back(weld.child);
            }
        }
    }

    // Delete the parent entity first to prevent infinite recursion
    for (auto &cdata : it->second) {
        cdata.deleter(cdata.component);
    }
    entityToComponents.erase(it);

    auto e_it = std::find(entities.begin(), entities.end(), id);
    if (e_it != entities.end()) {
        entities.erase(e_it);
    }

    // Recursively delete the children
    for (EntityId childId : childrenToDelete) {
        deleteEntity(childId);
    }
}

std::vector<EntityId> EntityManager::getAllEntities() {
    return entities;
}

}
