#pragma once

#include <map>
#include <glm/glm.hpp>
#include <cstdint>
#include <iostream>
#include <cmath>
#include <algorithm>
#include "SimpleEngine2D/core/System.hpp"
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/core/EventBus.hpp"
#include "SimpleEngine2D/events/CollisionEnter.hpp"
#include "SimpleEngine2D/components/Transform.hpp"
#include "SimpleEngine2D/components/RigidBody.hpp"
#include "SimpleEngine2D/components/Collider.hpp"

using CollisionKey = uint64_t;

struct AABB {
    glm::vec2 min; // top left
    glm::vec2 max; // bottom right
};

namespace simpleengine2d::systems {

class Collision : public core::System {

public:
    Collision() = default;
    ~Collision() = default;

    void init() override {};
    void update(float dt) override;
    void fixedUpdate(float f_dt) override;
    void clean() override {};

private:
    core::EntityManager &em = core::EntityManager::getInstance();

    AABB getColliderAABB(core::EntityId id);
    bool isColliding(AABB b1, AABB b2);
};

}
