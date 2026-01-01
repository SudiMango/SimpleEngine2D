#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <map>
#include <vector>
#include "SimpleEngine2D/core/System.hpp"
#include "SimpleEngine2D/core/EventBus.hpp"
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/components/Transform.hpp"
#include "SimpleEngine2D/components/Mesh.hpp"
#include "SimpleEngine2D/components/RigidBody.hpp"
#include "SimpleEngine2D/components/Collider.hpp"
#include "SimpleEngine2D/events/MouseButtonDown.hpp"
#include "SimpleEngine2D/events/CollisionEnter.hpp"

using namespace simpleengine2d;

namespace test_game::systems {

class GunSystem : public core::System {

public:
    GunSystem(core::EntityId id, core::EntityId player) : attachedEntity(id), player(player) {};
    ~GunSystem() = default;

    void init() override;
    void update(float dt) override;
    void clean() override {};

private:
    core::EntityId attachedEntity;
    core::EntityId player;
    core::EntityManager &em = core::EntityManager::getInstance();

    bool isFiring = false;
    std::map<core::EntityId, float> bullets;
    const float MAX_ALIVE_TIME = 5.0f;

};

}