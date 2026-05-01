#pragma once

#include <cstring>
#include <string>
#include "SimpleEngine2D/core/PersistenceManager.hpp"
#include "SimpleEngine2D/core/TagManager.hpp"
#include "imgui.h"
#include "core/Panel.hpp"

namespace simpleengine2d::editor {

class TopBar : public Panel {

public:
    TopBar(std::string currScene) : currScene(currScene) {}

    void init() override {}

    void update(float dt) override {
        ImGuiIO& io = ImGui::GetIO();

        if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_S) && !ImGui::IsAnyItemActive()) {
            pm.saveScene("GameScene");
        }

        if (io.KeyCtrl && ImGui::IsKeyPressed(ImGuiKey_O) && !ImGui::IsAnyItemActive()) {
            pm.loadScene("GameScene");
        }

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                    pm.saveScene("GameScene");
                }
                if (ImGui::MenuItem("Load", "Ctrl+O")) {
                    pm.loadScene("GameScene");
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Tags")) {
                if (ImGui::MenuItem("Create New Tag")) {
                    showCreateTagPopup = true;
                    memset(createTagInputBuffer, 0, sizeof(createTagInputBuffer));
                }
                if (ImGui::MenuItem("Delete tag")) {
                    showDestroyTagPopup = true;
                }
                ImGui::EndMenu();
            }

            ImGui::EndMainMenuBar();
        }

        if (showCreateTagPopup) {
            ImGui::OpenPopup("Create New Tag");
            showCreateTagPopup = false;
        }

        if (showDestroyTagPopup) {
            ImGui::OpenPopup("Delete Tag");
            showDestroyTagPopup = false;
        }

        if (ImGui::BeginPopupModal("Create New Tag", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            if (ImGui::IsWindowAppearing()) {
                ImGui::SetKeyboardFocusHere();
            }

            if (ImGui::InputText("##tagname", createTagInputBuffer, IM_ARRAYSIZE(createTagInputBuffer), ImGuiInputTextFlags_EnterReturnsTrue)) {
                if (strlen(createTagInputBuffer) > 0) {
                    tm.createTag(createTagInputBuffer);
                    ImGui::CloseCurrentPopup();
                }
            }

            ImGui::Separator();

            if (ImGui::Button("OK", ImVec2(120, 0))) {
                if (strlen(createTagInputBuffer) > 0) {
                    tm.createTag(createTagInputBuffer);
                }
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        if (ImGui::BeginPopupModal("Delete Tag", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            auto tags = tm.getAllTags();
            if (ImGui::BeginCombo("##TagCombo", tags.empty() ? "No tags in this scene..." : tags.at(0).c_str())) {
                for (int i = 0; i < tags.size(); i++) {
                    const bool isSelected = (tagIndexToDelete == i);

                    if (ImGui::Selectable(tags.at(i).c_str(), isSelected)) {
                        tagIndexToDelete = i;
                    }

                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }

            ImGui::Separator();

            if (ImGui::Button("Delete", ImVec2(120, 0))) {
                tm.destroyTag(tags.at(tagIndexToDelete));
                ImGui::CloseCurrentPopup();
            }

            ImGui::SameLine();

            if (ImGui::Button("Cancel", ImVec2(120, 0))) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

    void clean() override {}

private:
    std::string currScene;
    simpleengine2d::core::PersistenceManager &pm = simpleengine2d::core::PersistenceManager::getInstance();
    simpleengine2d::core::TagManager &tm = simpleengine2d::core::TagManager::getInstance();

    bool showCreateTagPopup = false;
    char createTagInputBuffer[64] = "";

    bool showDestroyTagPopup = false;
    int tagIndexToDelete = 0;

};

}