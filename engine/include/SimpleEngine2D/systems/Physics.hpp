#pragma once

#include <iostream>
#include <map>
#include "SimpleEngine2D/core/System.hpp"
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/core/EventBus.hpp"
#include "SimpleEngine2D/events/CollisionEnter.hpp"
#include "SimpleEngine2D/components/Transform.hpp"
#include "SimpleEngine2D/components/RigidBody.hpp"

namespace simpleengine2d::systems {

class Physics : public core::System {

public:
    void init() override {};
    void update(float dt) override {};
    void fixedUpdate(float f_dt) override;
    void clean() override {};

private:
    core::EntityManager &em = core::EntityManager::getInstance();

    std::map<core::EntityId, bool> canGoX;
    std::map<core::EntityId, bool> canGoY;

};

}
