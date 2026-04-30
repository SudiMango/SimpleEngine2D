#include <fstream>
#include "SimpleEngine2D/core/PersistenceManager.hpp"
#include "SimpleEngine2D/core/ComponentRegistry.hpp"
#include "SimpleEngine2D/util/glm_json.hpp"

namespace simpleengine2d::core {

void PersistenceManager::saveScene(const std::string& sceneName) {
    auto& registry = ComponentRegistry::getInstance();
    auto* cam = engine.getCamera();

    json saveFile;
    saveFile["sceneName"] = sceneName;
    saveFile["tags"] = tm.getAllTags();

    // entities
    saveFile["entities"] = json::array();
    for (auto& entityId : em.getAllEntities()) {
        json entityData;
        entityData["entityId"] = entityId;
        entityData["tags"] = tm.getTags(entityId);
        entityData["components"] = json::array();

        for (auto& name : registry.getComponentNames(entityId)) {
            entityData["components"].push_back(registry.serialize(name, entityId));
        }

        saveFile["entities"].push_back(entityData);
    }

    std::ofstream file("saves/" + sceneName + ".json");
    file << saveFile.dump(4);
    file.close();
}

void PersistenceManager::loadScene(const std::string& sceneName) {
    auto& registry = ComponentRegistry::getInstance();

    std::ifstream f("saves/" + sceneName + ".json");
    json data = json::parse(f);

    // reset state
    em.reset();
    tm.reset();

    // tags
    for (auto& tag : data["tags"].get<std::vector<std::string>>()) {
        tm.createTag(tag);
    }

    // entities
    for (auto& entityData : data["entities"]) {
        EntityId id = em.createEntity();

        if (entityData.contains("tags")) {
            for (auto& tag : entityData["tags"].get<std::vector<std::string>>()) {
                tm.addTag(id, tag);
            }
        }

        for (auto& componentData : entityData["components"]) {
            std::string name = componentData["componentName"].get<std::string>();
            registry.deserialize(name, id, componentData["properties"]);
        }
    }
}

}