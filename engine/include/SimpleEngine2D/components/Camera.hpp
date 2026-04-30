#pragma once

#include <glm/glm.hpp>
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/util/glm_json.hpp"
#include "SimpleEngine2D/util/sdl_json.hpp"

namespace simpleengine2d::components {

struct Camera {
    glm::vec2 position = glm::vec2(0.0f);

    core::EntityId follow = -1;
    glm::vec2 followOffset = glm::vec2(0.0f);

    bool shouldLerp = false;
    float smoothingFactor = 1.0f;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Camera, position, follow, followOffset, shouldLerp, smoothingFactor)

}
