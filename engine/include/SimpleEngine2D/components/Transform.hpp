#pragma once

#include <glm/glm.hpp>

namespace simpleengine2d::components {

struct TransformComponent {
    glm::vec2 position = glm::vec2(0.0f);
    glm::vec2 scale = glm::vec2(0.0f);
    float rotation = 0;
};

}