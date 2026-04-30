#pragma once

#include <string>
#include "SimpleEngine2D/core/PersistenceManager.hpp"
#include "imgui.h"
#include "core/Panel.hpp"

namespace simpleengine2d::editor {

class TopBar : public Panel {

public:
    TopBar(std::string currScene) : currScene(currScene) {}

    void init() override {}

    void update(float dt) override {
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Save", "Ctrl+S")) {
                pm.saveScene("GameScene");
            }
            if (ImGui::MenuItem("Load", "Ctrl+O")) {
                pm.loadScene("GameScene");
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }

    void clean() override {}

private:
    std::string currScene;
    simpleengine2d::core::PersistenceManager &pm = simpleengine2d::core::PersistenceManager::getInstance();

};

}