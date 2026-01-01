#include "SimpleEngine2D/systems/Collision.hpp"

namespace simpleengine2d::systems {

/**
 * Private functions
 */


/**
 * 1. Find the center of the entity relative to its top left (before rotation)
 * 2. Calculate the vector from the anchor point to that center
 * 3. Rotate that vector by the entity's current rotation
 * 4. Calculate the actual World Center (Anchor point + rotated vector)
 * 5. Return the top left and bottom right of the AABB
 */
AABB Collision::getColliderAABB(core::EntityId id) {
    components::TransformComponent* t = em.getComponent<components::TransformComponent>(id);
    components::ColliderComponent* c = em.getComponent<components::ColliderComponent>(id);

    float width = (c->scale.x > 0) ? c->scale.x : t->scale.x;
    float height = (c->scale.y > 0) ? c->scale.y : t->scale.y;

    glm::vec2 localCenter = { t->scale.x / 2.0f, t->scale.y / 2.0f };

    float dx = localCenter.x - t->anchor.x;
    float dy = localCenter.y - t->anchor.y;
    float angle = glm::radians(t->rotation);
    float rx = dx * cos(angle) - dy * sin(angle);
    float ry = dx * sin(angle) + dy * cos(angle);

    glm::vec2 worldCenter = {
        t->position.x + t->anchor.x + rx,
        t->position.y + t->anchor.y + ry
    };

    return {
        { worldCenter.x - (width / 2.0f), worldCenter.y - (height / 2.0f) },
        { worldCenter.x + (width / 2.0f), worldCenter.y + (height / 2.0f) }
    };
}

bool Collision::isColliding(AABB b1, AABB b2) {
    return b1.min.x < b2.max.x &&
           b1.max.x > b2.min.x &&
           b1.min.y < b2.max.y &&
           b1.max.y > b2.min.y;
}

/**
 * Public functions
 */

void Collision::update(float dt) {
    for (auto e : em.getAllEntities()) {
        if (em.hasComponent<components::TransformComponent>(e) && em.hasComponent<components::ColliderComponent>(e)) {
            components::TransformComponent *transform = em.getComponent<components::TransformComponent>(e);
            components::ColliderComponent *collider = em.getComponent<components::ColliderComponent>(e);

            if (collider->encompassTransform) {
                collider->scale = { 
                    (transform->scale.x * abs(cos(glm::radians(transform->rotation)))) + transform->scale.y * abs(sin(glm::radians(transform->rotation))), 
                    (transform->scale.x * abs(sin(glm::radians(transform->rotation)))) + transform->scale.y * abs(cos(glm::radians(transform->rotation)))
                };
            }
        }
    }
}

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
                    components::ColliderComponent *c1 = em.getComponent<components::ColliderComponent>(entity);
                    components::ColliderComponent *c2 = em.getComponent<components::ColliderComponent>(other);

                    auto it1 = std::find(c1->ignore.begin(), c1->ignore.end(), other);
                    auto it2 = std::find(c2->ignore.begin(), c2->ignore.end(), entity);
                    if (it1 != c1->ignore.end() || it2 != c2->ignore.end()) {
                        continue;
                    }

                    AABB box1 = getColliderAABB(entity);
                    AABB box2 = getColliderAABB(other);

                    if (isColliding(box1, box2)) {

                        float overlapLeft = box1.max.x - box2.min.x;
                        float overlapRight = box2.max.x - box1.min.x;
                        float overlapX = std::min(overlapLeft, overlapRight);

                        float overlapTop = box1.max.y - box2.min.y;
                        float overlapBottom = box2.max.y - box1.min.y;
                        float overlapY = std::min(overlapTop, overlapBottom);
                        
                        events::CollisionAxis axis;
                        float overlapPixels;

                        if (overlapX < overlapY) {
                            axis = events::CollisionAxis::X;
                            overlapPixels = (overlapLeft < overlapRight) ? overlapX : -overlapX;
                        } else {
                            axis = events::CollisionAxis::Y;
                            overlapPixels = (overlapTop < overlapBottom) ? overlapY : -overlapY;
                        }

                        if (!c1->isTrigger && !c2->isTrigger) {
                            if (em.hasComponent<components::RigidBodyComponent>(entity)) {
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

                            if (em.hasComponent<components::RigidBodyComponent>(other)) {
                                components::TransformComponent *transform = em.getComponent<components::TransformComponent>(other);
                                components::RigidBodyComponent *rb = em.getComponent<components::RigidBodyComponent>(other);
                                
                                if (axis == events::CollisionAxis::X) {
                                    transform->position.x += overlapPixels; // Note: Opposite direction if both have RB
                                    rb->velocity.x = 0;
                                } else {
                                    transform->position.y += overlapPixels;
                                    rb->velocity.y = 0;
                                }
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

}