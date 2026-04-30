#pragma once

#include <string>
#include <nlohmann/json.hpp>
#include "SimpleEngine2D/core/Engine.hpp"
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/core/TagManager.hpp"

using json = nlohmann::json;

namespace simpleengine2d::core {

class PersistenceManager {
public:
    static PersistenceManager& getInstance() {
        static PersistenceManager instance;
        return instance;
    }

    void saveScene(const std::string& sceneName);
    void loadScene(const std::string& sceneName);

private:
    PersistenceManager() = default;
    ~PersistenceManager() = default;

    PersistenceManager(const PersistenceManager&) = delete;
    PersistenceManager& operator=(const PersistenceManager&) = delete;

    core::Engine &engine = core::Engine::getInstance();
    core::TagManager &tm = core::TagManager::getInstance();
    core::EntityManager &em = core::EntityManager::getInstance();
};

}