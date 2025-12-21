#pragma once

#include <map>
#include <glm/glm.hpp>
#include <cstdint>
#include <iostream>
#include "SimpleEngine2D/core/System.hpp"
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/core/EventBus.hpp"
#include "SimpleEngine2D/events/CollisionEnter.hpp"
#include "SimpleEngine2D/events/CollisionExit.hpp"

using CollisionKey = uint64_t;

namespace simpleengine2d::systems {

class Collision : public core::System {

public:
    Collision() = default;
    ~Collision() = default;

    void init() override {};
    void update(float dt) override;
    void clean() override {};

private:
    core::EntityManager &em = core::EntityManager::getInstance();
    std::map<CollisionKey, util::CollisionAxis> history;

    CollisionKey createCollisionKey(core::EntityId a, core::EntityId b);
};

}
