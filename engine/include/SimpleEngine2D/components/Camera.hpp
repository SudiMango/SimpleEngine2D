#pragma once

#include <glm/glm.hpp>
#include "SimpleEngine2D/core/EntityManager.hpp"

namespace simpleengine2d::components {

struct Camera {
    glm::vec2 position = glm::vec2(0.0f);
    float zoom = 1.0f;

    core::EntityId follow = -1;
    glm::vec2 followOffset = glm::vec2(0.0f);

    bool shouldLerp = false;
    float smoothingFactor = 1.0f;
};

}
