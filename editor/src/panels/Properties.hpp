#pragma once

#include "SimpleEngine2D/core/ComponentRegistry.hpp"
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "imgui.h"
#include "core/Panel.hpp"

namespace simpleengine2d::editor {

class Properties : public Panel {

public:
    Properties(simpleengine2d::core::EntityId &selectedEntity) : selectedEntity(selectedEntity) {}

    void init() override {}

    void update(float dt) override {
        ImGui::Begin("Properties");
        if (selectedEntity == (simpleengine2d::core::EntityId)-1) {
            ImGui::TextDisabled("No entity selected.");
        } else {
            ImGui::Text("Entity %d", selectedEntity);
            ImGui::Separator();

            if (ImGui::Button("Add Component")) {
                ImGui::OpenPopup("add_component_popup");
            }
            if (ImGui::BeginPopup("add_component_popup")) {
                ImGui::Text("Components");
                ImGui::Separator();
                for (auto& name : cr.getAllComponentNames()) {
                    // grey out if entity already has it
                    bool alreadyHas = cr.hasComponent(name, selectedEntity);
                    if (alreadyHas) ImGui::BeginDisabled();
                    if (ImGui::Selectable(name.c_str())) {
                        cr.addDefault(name, selectedEntity);
                        ImGui::CloseCurrentPopup();
                    }
                    if (alreadyHas) ImGui::EndDisabled();
                }
                ImGui::EndPopup();
            }

            ImGui::Separator();

            for (auto& name : cr.getComponentNames(selectedEntity)) {
                ImGui::PushID(name.c_str());
                if (ImGui::CollapsingHeader(name.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
                    cr.inspect(name, selectedEntity);
                }
                ImGui::PopID();

                ImGui::Spacing();

                ImGui::PushID(name.c_str());
                if (ImGui::Button("Delete component")) {
                    cr.removeComponent(name, selectedEntity);
                }
                ImGui::PopID();

                ImGui::Spacing();
            }
        }
        ImGui::End();
    }

    void clean() override {}

private:
    simpleengine2d::core::EntityId &selectedEntity;
    simpleengine2d::core::ComponentRegistry &cr = simpleengine2d::core::ComponentRegistry::getInstance();
    simpleengine2d::core::EntityManager &em = simpleengine2d::core::EntityManager::getInstance();

};

}