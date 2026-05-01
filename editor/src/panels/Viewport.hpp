#pragma once

#include <SDL_render.h>
#include <SDL_video.h>
#include "SimpleEngine2D/components/Camera.hpp"
#include "imgui.h"
#include "core/Panel.hpp"
#include "core/EditorRenderer.hpp"
#include "core/Gizmo.hpp"
#include "SimpleEngine2D/components/Transform.hpp"
#include "SimpleEngine2D/components/Camera.hpp"

namespace simpleengine2d::editor {

enum class GizmoMode { Translate, Rotate, Scale };

class Viewport : public Panel {

public:
    Viewport(SDL_Window *window, SDL_Renderer *renderer, simpleengine2d::core::EntityId &selectedEntity) : editorRenderer(window, renderer), selectedEntity(selectedEntity) {}

    void init() override {
        editorRenderer.createRenderTarget(1280, 720);
        editorRenderer.init();
    }

    void update(float dt) override {
        ImGui::Begin("Viewport");

        if (ImGui::IsKeyPressed(ImGuiKey_W)) mode = GizmoMode::Translate;
        if (ImGui::IsKeyPressed(ImGuiKey_E)) mode = GizmoMode::Rotate;
        if (ImGui::IsKeyPressed(ImGuiKey_R)) mode = GizmoMode::Scale;

        ImGui::Text(" Gizmo mode: ");
        ImGui::SameLine();
        GizmoMode currentMode = mode;
        auto modeButton = [&](const char* label, GizmoMode m) {
            bool active = currentMode == m;
            if (active) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.5f, 0.9f, 1.0f));
            if (ImGui::Button(label)) mode = m;
            if (active) ImGui::PopStyleColor();
            ImGui::SameLine();
        };

        modeButton("Translate (W)", GizmoMode::Translate);
        modeButton("Rotate (E)", GizmoMode::Rotate);
        modeButton("Scale (R)", GizmoMode::Scale);
        ImGui::NewLine();

        ImVec2 size = ImGui::GetContentRegionAvail();
        if ((int)size.x != editorRenderer.targetWidth || (int)size.y != editorRenderer.targetHeight) {
            editorRenderer.createRenderTarget((int)size.x, (int)size.y);
        }

        editorRenderer.update(dt);
        ImGui::Image((ImTextureID)(intptr_t)editorRenderer.renderTarget, size);

        ImVec2 viewportPos = ImGui::GetItemRectMin();
        if (selectedEntity != -1) {
            auto camera = em.getComponent<simpleengine2d::components::Camera>(0);
            auto transform = em.getComponent<simpleengine2d::components::TransformComponent>(selectedEntity);

            if (camera && transform) {
                switch (mode) {
                    case GizmoMode::Translate:
                        simpleengine2d::editor::DrawTranslateGizmo(*transform, viewportPos);
                        break;
                    case GizmoMode::Rotate:
                        simpleengine2d::editor::DrawRotationGizmo(*transform, viewportPos);
                        break;
                    case GizmoMode::Scale:
                        simpleengine2d::editor::DrawScaleGizmo(*transform, viewportPos);
                        break;
                }
            }
        }
        ImGui::End();
    }

    void clean() override {
        editorRenderer.clean();
    }

private:
    simpleengine2d::core::EntityId &selectedEntity;
    editor::EditorRenderer editorRenderer;
    simpleengine2d::core::EntityManager &em = simpleengine2d::core::EntityManager::getInstance();
    GizmoMode mode = GizmoMode::Translate;

};

}