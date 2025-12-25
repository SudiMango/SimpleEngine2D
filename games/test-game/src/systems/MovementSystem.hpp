#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <map>

#include "SimpleEngine2D/core/System.hpp"
#include "SimpleEngine2D/core/EventBus.hpp"
#include "SimpleEngine2D/events/InputBegan.hpp"
#include "SimpleEngine2D/events/InputEnded.hpp"
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/components/Transform.hpp"
#include "SimpleEngine2D/components/RigidBody.hpp"

namespace test_game::systems {

class MovementSystem : public simpleengine2d::core::System {
public:
    explicit MovementSystem(simpleengine2d::core::EntityId id) : attachedEntity(id) {}
    ~MovementSystem() = default;

    void init() override;
    void update(float dt) override;
    void clean() override {}

private:
    simpleengine2d::core::EntityId attachedEntity;
    simpleengine2d::core::EntityManager &em = simpleengine2d::core::EntityManager::getInstance();

    glm::vec2 inputAxis = glm::vec2(0.0f);
    std::map<SDL_Keycode, bool> history;
};

}