#include "systems/TopDownMovementSystem.hpp"

namespace test_game::systems {

void TopDownMovementSystem::init() {
    core::EventBus::getInstance().subscribe<events::InputBegan>([this](void *evt){
        events::InputBegan *_evt = (events::InputBegan*)evt;

        history[_evt->keycode] = true;
        if (_evt->keycode == SDLK_w) {
            inputAxis.y = -1;
        } else if (_evt->keycode == SDLK_a) {
            inputAxis.x = -1;
        } else if (_evt->keycode == SDLK_s) {
            inputAxis.y = 1;
        } else if (_evt->keycode == SDLK_d) {
            inputAxis.x = 1;
        }
    });

    core::EventBus::getInstance().subscribe<events::InputEnded>([this](void *evt){
        events::InputEnded *_evt = (events::InputEnded*)evt;
        
        history[_evt->keycode] = false;
        if (_evt->keycode == SDLK_w) {
            auto it = history.find(SDLK_s);
            if (it != history.end() && it->second != false) {
                inputAxis.y = 1;
            } else {
                inputAxis.y = 0;
            }
        } else if (_evt->keycode == SDLK_a) {
            auto it = history.find(SDLK_d);
            if (it != history.end() && it->second != false) {
                inputAxis.x = 1;
            } else {
                inputAxis.x = 0;
            }
        } else if (_evt->keycode == SDLK_s) {
            auto it = history.find(SDLK_w);
            if (it != history.end() && it->second != false) {
                inputAxis.y = -1;
            } else {
                inputAxis.y = 0;
            }
        } else if (_evt->keycode == SDLK_d) {
            auto it = history.find(SDLK_a);
            if (it != history.end() && it->second != false) {
                inputAxis.x = -1;
            } else {
                inputAxis.x = 0;
            }
        }
    });
}

void TopDownMovementSystem::update(float dt) {
    if (em.hasComponent<components::TransformComponent>(attachedEntity) && em.hasComponent<components::RigidBodyComponent>(attachedEntity)) {
        components::TransformComponent *transform = em.getComponent<components::TransformComponent>(attachedEntity);
        components::RigidBodyComponent *rb = em.getComponent<components::RigidBodyComponent>(attachedEntity);
        
        glm::vec2 dir = inputAxis;
        if (glm::length(dir) > 0.0f) {
            dir = glm::normalize(dir);
        }

        rb->velocity = rb->maxVelocity * dir;
    }
} 

}