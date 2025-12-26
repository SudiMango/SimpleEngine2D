#pragma once

#include <map>
#include <glm/glm.hpp>
#include <cstdint>
#include <iostream>
#include "SimpleEngine2D/core/System.hpp"
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/core/EventBus.hpp"
#include "SimpleEngine2D/events/CollisionEnter.hpp"
#include "SimpleEngine2D/components/Transform.hpp"
#include "SimpleEngine2D/components/RigidBody.hpp"
#include "SimpleEngine2D/components/Collider.hpp"
#include "SimpleEngine2D/util/CollisionAxis.hpp"

using CollisionKey = uint64_t;

namespace simpleengine2d::systems {

class Collision : public core::System {

public:
    Collision() = default;
    ~Collision() = default;

    void init() override {};
    void update(float dt) override {};
    void fixedUpdate(float f_dt) override;
    void clean() override {};

private:
    core::EntityManager &em = core::EntityManager::getInstance();

    CollisionKey createCollisionKey(core::EntityId a, core::EntityId b);
    bool isColliding(components::TransformComponent *t1, components::TransformComponent *t2);
};

}
