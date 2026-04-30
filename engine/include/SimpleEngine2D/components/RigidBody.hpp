#pragma once

#include <glm/glm.hpp>
#include "SimpleEngine2D/util/glm_json.hpp"
#include "SimpleEngine2D/util/sdl_json.hpp"

namespace simpleengine2d::components {

struct RigidBodyComponent {
    glm::vec2 velocity = glm::vec2(0.0f);
    float maxVelocity = 1.0f;
    bool useGravity = false;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RigidBodyComponent, velocity, maxVelocity, useGravity)

}