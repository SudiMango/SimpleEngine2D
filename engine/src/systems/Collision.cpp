#include "SimpleEngine2D/systems/Collision.hpp"

namespace simpleengine2d::systems {

void Collision::fixedUpdate(float f_dt) {
    auto allEntities = em.getAllEntities();
    int iteration = 0;
    for (int i = 0; i < allEntities.size(); i++) {
        auto entity = allEntities[i];
        for (int j = iteration; j < allEntities.size(); j++) {
            auto other = allEntities[j];

            if (entity != other) {
                if (em.hasComponent<components::TransformComponent>(entity) && em.hasComponent<components::ColliderComponent>(entity) &&
                    em.hasComponent<components::TransformComponent>(other) && em.hasComponent<components::ColliderComponent>(other)) {

                    components::TransformComponent *t1 = em.getComponent<components::TransformComponent>(entity);
                    components::TransformComponent *t2 = em.getComponent<components::TransformComponent>(other);

                    if (isColliding(t1, t2)) {

                        float overlapLeft = (t1->position.x + t1->scale.x) - t2->position.x;
                        float overlapRight = (t2->position.x + t2->scale.x) - t1->position.x;
                        float overlapX = std::min(overlapLeft, overlapRight);

                        float overlapTop = (t1->position.y + t1->scale.y) - t2->position.y;
                        float overlapBottom = (t2->position.y + t2->scale.y) - t1->position.y;
                        float overlapY = std::min(overlapTop, overlapBottom);
                        events::CollisionAxis axis;
                        float overlapPixels;

                        if (overlapX < overlapY) {
                            axis = events::CollisionAxis::X;
                            if (overlapLeft < overlapRight) {
                                overlapPixels = overlapX;
                            } else {
                                overlapPixels = -overlapX;
                            }
                        } else {
                            axis = events::CollisionAxis::Y;
                            if (overlapTop < overlapBottom) {
                                overlapPixels = overlapY;
                            } else {
                                overlapPixels = -overlapY;
                            }
                        }

                        if (em.hasComponent<components::TransformComponent>(entity) && em.hasComponent<components::RigidBodyComponent>(entity)) {
                            components::TransformComponent *transform = em.getComponent<components::TransformComponent>(entity);
                            components::RigidBodyComponent *rb = em.getComponent<components::RigidBodyComponent>(entity);
                            
                            if (axis == events::CollisionAxis::X) {
                                transform->position.x -= overlapPixels;
                                rb->velocity.x = 0;
                            } else {
                                transform->position.y -= overlapPixels;
                                rb->velocity.y = 0;
                            }
                        }

                        if (em.hasComponent<components::TransformComponent>(other) && em.hasComponent<components::RigidBodyComponent>(other)) {
                            components::TransformComponent *transform = em.getComponent<components::TransformComponent>(other);
                            components::RigidBodyComponent *rb = em.getComponent<components::RigidBodyComponent>(other);
                            
                            if (axis == events::CollisionAxis::X) {
                                transform->position.x -= overlapPixels;
                                rb->velocity.x = 0;
                            } else {
                                transform->position.y -= overlapPixels;
                                rb->velocity.y = 0;
                            }
                        }

                        events::CollisionEnter ce;
                        ce.entity1 = entity;
                        ce.entity2 = other;
                        ce.axis = axis;
                        core::EventBus::getInstance().publish<events::CollisionEnter>(&ce);

                    }
                }
            }
        }
        iteration++;
    }
}

bool Collision::isColliding(components::TransformComponent *t1, components::TransformComponent *t2) {
    return t1->position.x < t2->position.x + t2->scale.x &&
        t1->position.x + t1->scale.x > t2->position.x &&
        t1->position.y < t2->position.y + t2->scale.y &&
        t1->position.y + t1->scale.y > t2->position.y;
}

}