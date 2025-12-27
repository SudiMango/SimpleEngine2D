#pragma once

#include <glm/glm.hpp>

namespace simpleengine2d::events {

struct MouseMotion {
    glm::vec2 position = glm::vec2(0.0f);
};

}