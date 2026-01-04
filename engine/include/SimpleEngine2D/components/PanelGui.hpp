#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

namespace simpleengine2d::components {

struct PanelGuiComponent {
    glm::vec2 position = glm::vec2(0.0f);
    glm::vec2 scale = glm::vec2(0.0f);
    glm::ivec4 color = {255, 255, 255, 255};
    SDL_Rect rect = {0, 0, 1, 1};
    bool inWorldSpace = false;
    int zIndex = 100;
};

}