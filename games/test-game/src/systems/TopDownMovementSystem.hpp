#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include <map>
#include "SimpleEngine2D/core/System.hpp"
#include "SimpleEngine2D/core/EventBus.hpp"
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/components/Transform.hpp"
#include "SimpleEngine2D/components/RigidBody.hpp"
#include "SimpleEngine2D/events/InputBegan.hpp"
#include "SimpleEngine2D/events/InputEnded.hpp"

using namespace simpleengine2d;

namespace test_game::systems {

class TopDownMovementSystem : public core::System {

public:
    TopDownMovementSystem(core::EntityId id) : attachedEntity(id) {};
    ~TopDownMovementSystem() = default;

    void init() override;
    void update(float dt) override;
    void clean() override {};

private:
    core::EntityId attachedEntity;
    core::EntityManager &em = core::EntityManager::getInstance();

    glm::vec2 inputAxis = glm::vec2(0.0f);
    std::map<SDL_Keycode, bool> history;

};

}