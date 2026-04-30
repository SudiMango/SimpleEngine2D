#pragma once

#include <SDL_render.h>
#include <SDL_video.h>
#include "imgui.h"
#include "core/Panel.hpp"
#include "core/EditorRenderer.hpp"

namespace simpleengine2d::editor {

class Viewport : public Panel {

public:
    Viewport(SDL_Window *window, SDL_Renderer *renderer) : editorRenderer(window, renderer) {}

    void init() override {
        editorRenderer.createRenderTarget(1280, 720);
        editorRenderer.init();
    }

    void update(float dt) override {
        ImGui::Begin("Viewport");
        ImVec2 size = ImGui::GetContentRegionAvail();

        if ((int)size.x != editorRenderer.targetWidth || (int)size.y != editorRenderer.targetHeight) {
            editorRenderer.createRenderTarget((int)size.x, (int)size.y);
        }

        editorRenderer.update(dt);

        ImGui::Image((ImTextureID)(intptr_t)editorRenderer.renderTarget, size);
        ImGui::End();
    }

    void clean() override {
        editorRenderer.clean();
    }

private:
    editor::EditorRenderer editorRenderer;

};

}