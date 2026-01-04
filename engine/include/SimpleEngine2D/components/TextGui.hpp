#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

namespace simpleengine2d::components {

struct TextGuiComponent {
    glm::vec2 position = glm::vec2(0.0f);
    char *text = nullptr;
    char *fontPath = nullptr;
    int textSize = 24;
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Texture *texture;
    bool inWorldSpace = false;
    int zIndex = 100;
};

}