#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

namespace simpleengine2d::components {

struct TransformComponent {
    glm::vec2 position = glm::vec2(0.0f);
    glm::vec2 scale = glm::vec2(0.0f);
    float rotation = 0;
    SDL_Point anchor = {0, 0};
};

}