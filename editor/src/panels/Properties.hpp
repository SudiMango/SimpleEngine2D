#pragma once

#include "SimpleEngine2D/core/ComponentRegistry.hpp"
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/core/TagManager.hpp"
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
            ImGui::End();
            return;
        }

        ImGui::Text("Entity %d", selectedEntity);
        ImGui::Separator();

        if (ImGui::BeginTabBar("PropertyTabs")) {

            if (ImGui::BeginTabItem("Components")) {
                if (ImGui::Button("+ Add Component")) {
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
                        ImGui::Spacing();
                        ImGui::Separator();
                        ImGui::Spacing();
                        if (ImGui::Button("Delete component")) {
                            cr.removeComponent(name, selectedEntity);
                        }
                    }
                    ImGui::PopID();
                }

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("Tags")) {
                if (ImGui::Button("+ Add Tag")) {
                    ImGui::OpenPopup("add_tag_popup");
                }

                if (ImGui::BeginPopup("add_tag_popup")) {
                    ImGui::Text("Tags");
                    ImGui::Separator();
                    for (auto &tag : tm.getAllTags()) {
                        bool alreadyHas = tm.hasTag(selectedEntity, tag);
                        if (alreadyHas) ImGui::BeginDisabled();
                        if (ImGui::Selectable(tag.c_str())) {
                            tm.addTag(selectedEntity, tag);
                            ImGui::CloseCurrentPopup();
                        }
                        if (alreadyHas) ImGui::EndDisabled();
                    }
                    ImGui::EndPopup();
                }

                ImGui::Separator();

                for (auto &tag : tm.getAllTags()) {
                    if (tm.hasTag(selectedEntity, tag)) {
                        ImGui::Text("%s", tag.c_str());

                        ImGui::SameLine();

                        ImGui::PushID(tag.c_str());
                        if (ImGui::SmallButton("x")) {
                            tm.removeTag(selectedEntity, tag);
                        }
                        ImGui::PopID();
                    }
                }

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::End();
    }

    void clean() override {}

private:
    simpleengine2d::core::EntityId &selectedEntity;
    simpleengine2d::core::ComponentRegistry &cr = simpleengine2d::core::ComponentRegistry::getInstance();
    simpleengine2d::core::EntityManager &em = simpleengine2d::core::EntityManager::getInstance();
    simpleengine2d::core::TagManager &tm = simpleengine2d::core::TagManager::getInstance();

};

}