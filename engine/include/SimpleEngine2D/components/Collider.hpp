#pragma once

#include <vector>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>

namespace simpleengine2d::components {

struct ColliderComponent {
    SDL_Rect rect = {0, 0, 1, 1};
    glm::ivec4 color = glm::ivec4(255, 0, 0, 255);
    bool showOutline = false;

    glm::vec2 scale = glm::vec2(0.0f);
    bool encompassTransform = false;

    bool isTrigger = false;
    
    std::vector<core::EntityId> ignore;
};

}