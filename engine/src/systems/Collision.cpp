#include "SimpleEngine2D/systems/Collision.hpp"
#include "SimpleEngine2D/components/Transform.hpp"
#include "SimpleEngine2D/components/Collider.hpp"

namespace simpleengine2d::systems {

void Collision::update(float dt) {
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
                    CollisionKey key = createCollisionKey(entity, other);
                    auto it = history.find(key);

                    if (t1->position.x < t2->position.x + t2->scale.x &&
                        t1->position.x + t1->scale.x > t2->position.x &&
                        t1->position.y < t2->position.y + t2->scale.y &&
                        t1->position.y + t1->scale.y > t2->position.y) {

                        if (it == history.end()) {
                            events::CollisionEnter ce;

                            float overlapLeft = (t1->position.x + t1->scale.x) - t2->position.x;
                            float overlapRight = (t2->position.x + t2->scale.x) - t1->position.x;
                            float overlapX = std::min(overlapLeft, overlapRight);

                            float overlapTop = (t1->position.y + t1->scale.y) - t2->position.y;
                            float overlapBottom = (t2->position.y + t2->scale.y) - t1->position.y;
                            float overlapY = std::min(overlapTop, overlapBottom);

                            if (overlapX < overlapY) {
                                history[key] = simpleengine2d::util::CollisionAxis::X;
                                if (overlapLeft < overlapRight) {
                                    ce = {entity, other, simpleengine2d::util::CollisionAxis::X, overlapX};
                                } else {
                                    ce = {entity, other, simpleengine2d::util::CollisionAxis::X, -overlapX};
                                }
                            } else {
                                history[key] = simpleengine2d::util::CollisionAxis::Y;
                                if (overlapTop < overlapBottom) {
                                    ce = {entity, other, simpleengine2d::util::CollisionAxis::Y, overlapY};
                                } else {
                                    ce = {entity, other, simpleengine2d::util::CollisionAxis::Y, -overlapY};
                                }
                            }
                            core::EventBus::getInstance().publish<events::CollisionEnter>(&ce);
                        }
                    } else {
                        if (it != history.end()) {
                            events::CollisionExit ce{entity, other, it->second};
                            history.erase(key);
                            core::EventBus::getInstance().publish<events::CollisionExit>(&ce);
                        }
                    }
                }
            }
        }
        iteration++;
    }
}

CollisionKey Collision::createCollisionKey(core::EntityId a, core::EntityId b) {
    if (a > b) {
        std::swap(a, b);
    }

    return (uint64_t(a) << 32) | uint64_t(b);
}

}