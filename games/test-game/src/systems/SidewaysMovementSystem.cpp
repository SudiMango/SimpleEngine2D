#include "SidewaysMovementSystem.hpp"

namespace test_game::systems {

void SidewaysMovementSystem::init() {
    core::EventBus::getInstance().subscribe<events::InputBegan>(this, [this](void *evt){
        events::InputBegan *_evt = (events::InputBegan*)evt;

        history[_evt->keycode] = true;
        if (_evt->keycode == SDLK_a) {
            inputAxis = -1;
        } else if (_evt->keycode == SDLK_d) {
            inputAxis = 1;
        }
    });

    core::EventBus::getInstance().subscribe<events::InputEnded>(this, [this](void *evt){
        events::InputEnded *_evt = (events::InputEnded*)evt;
        
        history[_evt->keycode] = false;
        if (_evt->keycode == SDLK_a) {
            auto it = history.find(SDLK_d);
            if (it != history.end() && it->second != false) {
                inputAxis = 1;
            } else {
                inputAxis = 0;
            }
        } else if (_evt->keycode == SDLK_d) {
            auto it = history.find(SDLK_a);
            if (it != history.end() && it->second != false) {
                inputAxis = -1;
            } else {
                inputAxis = 0;
            }
        }
    });

    core::EventBus::getInstance().subscribe<events::CollisionEnter>(this, [this](void *evt) {
        events::CollisionEnter *_evt = (events::CollisionEnter*)evt;
        if (_evt->entity1 != attachedEntity && _evt->entity2 != attachedEntity) {
            return;
        }

        core::EntityId otherEntity = (_evt->entity1 == attachedEntity) ? _evt->entity2 : _evt->entity1;
        if (core::TagManager::getInstance().hasTag(otherEntity, "ground")) {
            isGrounded = true;
        }
    });

    jumpAudio.filePath = (char*)"./games/test-game/assets/audio/jump.mp3";
}

void SidewaysMovementSystem::update(float dt) {
    if (em.hasComponent<components::TransformComponent>(attachedEntity) && em.hasComponent<components::RigidBodyComponent>(attachedEntity)) {
        components::TransformComponent *transform = em.getComponent<components::TransformComponent>(attachedEntity);
        components::RigidBodyComponent *rb = em.getComponent<components::RigidBodyComponent>(attachedEntity);
        components::MeshComponent *mesh = em.getComponent<components::MeshComponent>(attachedEntity);

        if (isDashing) {
            rb->velocity.x = rb->maxVelocity * 3 * dashDir;
            dashTimer += dt;
            if (dashTimer >= MAX_DASH_TIME) {
                isDashing = false;
                canDash = true;
                dashTimer = 0.0f;
                dashDir = 1;
            }
        } else {
            rb->velocity.x = rb->maxVelocity * inputAxis;
        }

        auto it_space = history.find(SDLK_SPACE);
        if (it_space != history.end() && it_space->second == true && isGrounded) {
            isGrounded = false;
            rb->velocity.y -= 1500.0f;
            events::PlayAudio audio;
            audio.audio = jumpAudio;
            core::EventBus::getInstance().publish<events::PlayAudio>(&audio);
        }

        auto it_lshift = history.find(SDLK_LSHIFT);
        if (it_lshift != history.end() && it_lshift->second == true && canDash) {
            canDash = false;
            isDashing = true;
            dashDir = (mesh->flipX) ? -1 : 1;
        }
    }
}

void SidewaysMovementSystem::clean() {
    core::EventBus::getInstance().unsubscribe(this);
}

}
