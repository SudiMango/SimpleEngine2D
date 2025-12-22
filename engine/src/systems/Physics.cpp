#include "SimpleEngine2D/systems/Physics.hpp"
#include "SimpleEngine2D/components/Transform.hpp"
#include "SimpleEngine2D/components/RigidBody.hpp"

namespace simpleengine2d::systems {

void Physics::init() {
    core::EventBus::getInstance().subscribe<events::CollisionEnter>([this](void *evt) {
        events::CollisionEnter *_evt = (events::CollisionEnter*)evt;
        std::cout << "Collision entered between " << _evt->entity1 << " and " << _evt->entity2 << " on axis " << _evt->axis << " by " << _evt->overlapPixels << " pixels." << std::endl; 

        if (em.hasComponent<components::TransformComponent>(_evt->entity1) && em.hasComponent<components::RigidBodyComponent>(_evt->entity1)) {
            components::TransformComponent *transform = em.getComponent<components::TransformComponent>(_evt->entity1);
            components::RigidBodyComponent *rb = em.getComponent<components::RigidBodyComponent>(_evt->entity1);
            
            if (_evt->axis == simpleengine2d::util::CollisionAxis::X) {
                canGoX[_evt->entity1] = false;
                transform->position.x -= _evt->overlapPixels;
                rb->velocity.x = 0;
            } else {
                canGoY[_evt->entity1] = false;
                transform->position.y -= _evt->overlapPixels;
                rb->velocity.y = 0;
            }
        }

        if (em.hasComponent<components::TransformComponent>(_evt->entity2) && em.hasComponent<components::RigidBodyComponent>(_evt->entity2)) {
            components::TransformComponent *transform = em.getComponent<components::TransformComponent>(_evt->entity2);
            components::RigidBodyComponent *rb = em.getComponent<components::RigidBodyComponent>(_evt->entity2);
            
            if (_evt->axis == simpleengine2d::util::CollisionAxis::X) {
                canGoX[_evt->entity2] = false;
                transform->position.x -= _evt->overlapPixels;
                rb->velocity.x = 0;
            } else {
                canGoY[_evt->entity2] = false;
                transform->position.y -= _evt->overlapPixels;
                rb->velocity.y = 0;
            }
        }
    });

    core::EventBus::getInstance().subscribe<events::CollisionExit>([this](void *evt) {
        events::CollisionExit *_evt = (events::CollisionExit*)evt;
        std::cout << "Collision exited between " << _evt->entity1 << " and " << _evt->entity2 << " on axis " << _evt->axis << std::endl;
        if (_evt->axis == simpleengine2d::util::CollisionAxis::X) {
            canGoX[_evt->entity1] = true;
            canGoX[_evt->entity2] = true;
        } else {
            canGoY[_evt->entity1] = true;
            canGoY[_evt->entity2] = true;
        }
    });
}


void Physics::update(float dt) {
    for (auto entity : em.getAllEntities()) {
        if (em.hasComponent<components::TransformComponent>(entity) && em.hasComponent<components::RigidBodyComponent>(entity)) {
            components::TransformComponent *transform = em.getComponent<components::TransformComponent>(entity);
            components::RigidBodyComponent *rb = em.getComponent<components::RigidBodyComponent>(entity);
            
            // if (rb->useGravity) {
            //     rb->velocity.y += 10;
            // }

            auto it_x = canGoX.find(entity);
            if (it_x == canGoX.end() || (it_x != canGoX.end() && it_x->second == true)) {
                transform->position.x += rb->velocity.x * dt;
            }

            auto it_y = canGoY.find(entity);
            if (it_y == canGoY.end() || (it_y != canGoY.end() && it_y->second == true)) {
                transform->position.y += rb->velocity.y * dt;
            }

        }
    }
}

}