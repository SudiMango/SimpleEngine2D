#include "systems/TopDownLookingSystem.hpp"

namespace test_game::systems {

void TopDownLookingSystem::init() {
    core::EventBus::getInstance().subscribe<events::MouseMotion>(this, [this](void *evt) {
        events::MouseMotion *_evt = (events::MouseMotion*)evt;
        currMousePos = _evt->position;
    });
}

void TopDownLookingSystem::update(float dt) {
    if (em.hasComponent<components::TransformComponent>(attachedEntity) && em.hasComponent<components::MeshComponent>(attachedEntity)) {
        components::TransformComponent *transform = em.getComponent<components::TransformComponent>(attachedEntity);
        
        core::EntityId cameraEntity = em.getAllEntities()[0];
        components::Camera *camera = em.getComponent<components::Camera>(cameraEntity);
        
        glm::vec2 mouseWorldPos = {currMousePos.x + camera->position.x, currMousePos.y + camera->position.y};
        glm::vec2 center = {transform->position.x + (transform->scale.x / 2), transform->position.y + (transform->scale.y / 2)};
        glm::vec2 dir = mouseWorldPos - center;
        
        float angle = glm::degrees(glm::atan(dir.y, dir.x));
        transform->rotation = angle;
    }
}

void TopDownLookingSystem::clean() {
    core::EventBus::getInstance().unsubscribe(this);
}

}