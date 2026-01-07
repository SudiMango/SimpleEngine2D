#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <iostream>
#include <map>
#include "SimpleEngine2D/core/System.hpp"
#include "SimpleEngine2D/core/EventBus.hpp"
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/core/TagManager.hpp"
#include "SimpleEngine2D/components/Audio.hpp"
#include "SimpleEngine2D/components/Transform.hpp"
#include "SimpleEngine2D/components/RigidBody.hpp"
#include "SimpleEngine2D/components/Mesh.hpp"
#include "SimpleEngine2D/events/InputBegan.hpp"
#include "SimpleEngine2D/events/InputEnded.hpp"
#include "SimpleEngine2D/events/CollisionEnter.hpp"
#include "SimpleEngine2D/events/PlayAudio.hpp"

using namespace simpleengine2d;

namespace test_game::systems {

class SidewaysMovementSystem : public core::System {

public:
    SidewaysMovementSystem(core::EntityId id) : attachedEntity(id) {};
    ~SidewaysMovementSystem() = default;

    void init() override;
    void update(float dt) override;
    void clean() override;

private:
    core::EntityId attachedEntity;
    core::EntityManager &em = core::EntityManager::getInstance();

    bool isGrounded = false;
    float inputAxis = 0.0f;
    std::map<SDL_Keycode, bool> history;

    float dashTimer = 0.0f;
    const float MAX_DASH_TIME = 0.3f;
    bool canDash = true;
    bool isDashing = false;
    int dashDir = 1;

    components::AudioComponent jumpAudio;

};

}