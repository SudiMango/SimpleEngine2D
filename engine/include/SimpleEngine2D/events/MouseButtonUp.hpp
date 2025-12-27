#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

namespace simpleengine2d::events {

struct MouseButtonUp {
    Uint8 button;
    glm::vec2 position = glm::vec2(0.0f);
};

}