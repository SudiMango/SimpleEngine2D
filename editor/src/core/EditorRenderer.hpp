#pragma once

#include <SDL2/SDL.h>
#include "SimpleEngine2D/systems/Render.hpp"

namespace simpleengine2d::editor {

class EditorRenderer : public simpleengine2d::systems::Render {
public:
    SDL_Texture* renderTarget = nullptr;
    int targetWidth = 0;
    int targetHeight = 0;

    EditorRenderer(SDL_Window* window, SDL_Renderer* externalRenderer): Render(window, -1, 0) {
        renderer = externalRenderer;
    }

    void createRenderTarget(int width, int height) {
        if (renderTarget) {
            SDL_DestroyTexture(renderTarget);
        }

        renderTarget = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            width, height
        );

        targetWidth = width;
        targetHeight = height;
    }

    void update(float dt) override {
        if (!renderTarget) return;

        SDL_SetRenderTarget(renderer, renderTarget);
        renderScene(dt);
        SDL_SetRenderTarget(renderer, nullptr);
    }

    void clean() override {
        if (renderTarget) {
            SDL_DestroyTexture(renderTarget);
            renderTarget = nullptr;
        }
    }
};

}