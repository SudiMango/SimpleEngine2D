#include "SidewaysLookingSystem.hpp"

namespace test_game::systems {

void SidewaysLookingSystem::init() {
    core::EventBus::getInstance().subscribe<events::MouseMotion>(this, [this](void *evt) {
        events::MouseMotion *_evt = (events::MouseMotion*)evt;
        currMousePos = _evt->position;
    });
}

void SidewaysLookingSystem::update(float dt) {
    core::EntityId camera = em.getAllEntities()[0];
    components::Camera *camComp = em.getComponent<components::Camera>(camera);

    // Flipping the character
    if (em.hasComponent<components::TransformComponent>(attachedEntity) && em.hasComponent<components::MeshComponent>(attachedEntity)) {
        components::TransformComponent *transform = em.getComponent<components::TransformComponent>(attachedEntity);
        components::MeshComponent *mesh = em.getComponent<components::MeshComponent>(attachedEntity);

        if (currMousePos.x >= (transform->position.x + (transform->scale.x/2)) - (camComp->followOffset.x/2) && mesh->flipX == true) {
            mesh->flipX = false;
            camComp->followOffset.x *= -1;
            em.getComponent<components::MeshComponent>(gun)->flipY = false;
            facingRight = true;
            
            if (em.hasComponent<components::WeldComponent>(attachedEntity)) {
                components::WeldComponent *weld = em.getComponent<components::WeldComponent>(attachedEntity);

                for (auto &w : weld->welds) {
                    w.positionOffset.x += transform->scale.x;
                }
            }

        } else if (currMousePos.x < (transform->position.x + (transform->scale.x/2)) - (camComp->followOffset.x/2) && mesh->flipX == false) {
            mesh->flipX = true;
            camComp->followOffset.x *= -1;
            em.getComponent<components::MeshComponent>(gun)->flipY = true;
            facingRight = false;

            if (em.hasComponent<components::WeldComponent>(attachedEntity)) {
                components::WeldComponent *weld = em.getComponent<components::WeldComponent>(attachedEntity);

                for (auto &w : weld->welds) {
                    w.positionOffset.x -= transform->scale.x;
                }
            }
        }
    }

    // Rotating the gun
    if (em.hasComponent<components::TransformComponent>(gun)) {
        components::TransformComponent *transform = em.getComponent<components::TransformComponent>(gun);

        glm::vec2 mouseWorldPos = {currMousePos.x + camComp->position.x, currMousePos.y + camComp->position.y};
        glm::vec2 center = {transform->position.x + (transform->scale.x / 2), transform->position.y + (transform->scale.y / 2)};
        glm::vec2 dir = mouseWorldPos - center;
        
        float angle = glm::degrees(glm::atan(dir.y, dir.x));
        if (facingRight) {
            angle = std::clamp(angle, -90.0f, 90.0f);
        } else {
            // normalize to -180, 180
            if (angle >= 180.0f) {
                angle -= 360.0f;
            }
            if (angle <  -180.0f) {
                angle += 360.0f;
            }

            // clamp to [-180,-90] U [90,180]
            if (angle > -90.0f && angle < 90.0f) {
                angle = (angle < 0.0f) ? -90.0f : 90.0f;
            }
        }
        transform->rotation = angle;
    }
}

void SidewaysLookingSystem::clean() {
    core::EventBus::getInstance().unsubscribe(this);
}

}