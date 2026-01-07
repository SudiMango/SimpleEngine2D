#pragma once

#include <iostream>
#include <algorithm>
#include <glm/glm.hpp>
#include "SimpleEngine2D/core/System.hpp"
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/core/EventBus.hpp"
#include "SimpleEngine2D/components/Transform.hpp"
#include "SimpleEngine2D/components/Camera.hpp"
#include "SimpleEngine2D/components/Mesh.hpp"
#include "SimpleEngine2D/components/Weld.hpp"
#include "SimpleEngine2D/events/MouseMotion.hpp"

using namespace simpleengine2d;

namespace test_game::systems {

class SidewaysLookingSystem : public core::System {

public:
    SidewaysLookingSystem(core::EntityId player, core::EntityId gun) : attachedEntity(player), gun(gun) {};
    ~SidewaysLookingSystem() = default;

    void init() override;
    void update(float dt) override;
    void clean() override {};

private:
    core::EntityId attachedEntity;
    core::EntityManager &em = core::EntityManager::getInstance();

    glm::vec2 currMousePos = {0, 0};
    core::EntityId gun;
    bool facingRight = true;

};

}