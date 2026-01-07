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

    void switchScene(int index) {
        if (index >= 0 && index < scenes.size()) {
            if (currSceneIndex != -1) {
                scenes[currSceneIndex]->clean();
            }
            currSceneIndex = index;
            scenes[index]->setup();
        }
    }

private:
    SceneManager() = default;
    ~SceneManager() = default;

    SceneManager(const SceneManager&) = delete;
    SceneManager& operator=(const SceneManager&) = delete;

    std::vector<core::Scene*> scenes;
    int currSceneIndex = -1;


};

}