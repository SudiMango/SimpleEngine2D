#include "SimpleEngine2D/systems/Physics.hpp"

namespace simpleengine2d::systems {

void Physics::fixedUpdate(float f_dt) {
    for (auto entity : em.getAllEntities()) {
        if (em.hasComponent<components::TransformComponent>(entity) && em.hasComponent<components::RigidBodyComponent>(entity)) {
            components::TransformComponent *transform = em.getComponent<components::TransformComponent>(entity);
            components::RigidBodyComponent *rb = em.getComponent<components::RigidBodyComponent>(entity);
            
            if (rb->useGravity) {
                rb->velocity.y += 98.0f;
            }

            auto it_x = canGoX.find(entity);
            if (it_x == canGoX.end() || (it_x != canGoX.end() && it_x->second == true)) {
                transform->position.x += rb->velocity.x * f_dt;
            }

            auto it_y = canGoY.find(entity);
            if (it_y == canGoY.end() || (it_y != canGoY.end() && it_y->second == true)) {
                transform->position.y += rb->velocity.y * f_dt;
            }
            
        }
    }
}

}