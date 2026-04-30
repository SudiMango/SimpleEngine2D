#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "SimpleEngine2D/util/glm_json.hpp"
#include "SimpleEngine2D/util/sdl_json.hpp"

namespace simpleengine2d::components {

struct TransformComponent {
    glm::vec2 position = glm::vec2(0.0f);
    glm::vec2 scale = glm::vec2(0.0f);
    float rotation = 0;
    SDL_Point anchor = {0, 0};
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TransformComponent, position, scale, rotation, anchor)

}