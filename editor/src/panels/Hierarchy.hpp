#pragma once

#include <string>
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "imgui.h"
#include "core/Panel.hpp"

namespace simpleengine2d::editor {

class Hierarchy : public Panel {

public:
    Hierarchy(simpleengine2d::core::EntityId &selectedEntity) : selectedEntity(selectedEntity) {}

    void init() override {}

    void update(float dt) override {
        ImGui::Begin("Hierarchy");
        if (ImGui::Button("Add Entity")) {
            em.createEntity();
        }ImGui::SameLine();
        if (ImGui::Button("Delete Entity")) {
            em.deleteEntity(selectedEntity);
        }
        ImGui::Separator();
        for (auto entity : em.getAllEntities()) {
            bool isSelected = (selectedEntity == entity);
            std::string entityName = "Entity " + std::to_string(entity);
            if (ImGui::Selectable(entityName.c_str(), isSelected)) {
                selectedEntity = entity;
            }
        }
        ImGui::End();
    }

    void clean() override {}

private:
    simpleengine2d::core::EntityId &selectedEntity;
    simpleengine2d::core::EntityManager &em = simpleengine2d::core::EntityManager::getInstance();

};

}