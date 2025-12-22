#pragma once

#include <glm/glm.hpp>

namespace simpleengine2d::components {

struct RigidBodyComponent {
    glm::vec2 velocity = glm::vec2(0.0f);
    float maxVelocity = 1.0f;
    bool useGravity = false;
};

}