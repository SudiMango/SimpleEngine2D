#include "systems/MovementSystem.hpp"

namespace test_game::systems {

void MovementSystem::init() {
    simpleengine2d::core::EventBus::getInstance().subscribe<simpleengine2d::events::InputBegan>([this](void *evt){
        auto *_evt = static_cast<simpleengine2d::events::InputBegan*>(evt);

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

    simpleengine2d::core::EventBus::getInstance().subscribe<simpleengine2d::events::InputEnded>([this](void *evt){
        auto *_evt = static_cast<simpleengine2d::events::InputEnded*>(evt);
        
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

void MovementSystem::update(float dt) {
    if (em.hasComponent<simpleengine2d::components::TransformComponent>(attachedEntity) &&
        em.hasComponent<simpleengine2d::components::RigidBodyComponent>(attachedEntity)) {
        auto *transform = em.getComponent<simpleengine2d::components::TransformComponent>(attachedEntity);
        auto *rb = em.getComponent<simpleengine2d::components::RigidBodyComponent>(attachedEntity);
        
        glm::vec2 dir = inputAxis;
        if (glm::length(dir) > 0.0f) {
            dir = glm::normalize(dir);
        }

        rb->velocity = rb->maxVelocity * dir;
    }
}

}