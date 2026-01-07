#pragma once

#include <iostream>
#include <vector>
#include "SimpleEngine2D/core/Scene.hpp"
#include "SimpleEngine2D/core/Engine.hpp"

namespace simpleengine2d::core {

class SceneManager {

public:
    static SceneManager &getInstance() {
        static SceneManager instance;
        return instance;
    }

    void addScene(core::Scene *scene) {
        scenes.push_back(scene);
    }

    void init(int index) {
        scenes[index]->setup();
    }

    core::Scene *getScene(int index) {
        return scenes[index];
    }

private:
    SceneManager() = default;
    ~SceneManager() = default;

    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    std::vector<core::Scene*> scenes;


};

}