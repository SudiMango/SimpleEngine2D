#pragma once

#include <SDL2/SDL.h>

namespace simpleengine2d::components {

struct ColliderComponent {
    SDL_Rect rect = {0, 0, 1, 1};
    glm::ivec4 color = glm::ivec4(255, 0, 0, 255);
    bool showOutline = false;
};

}