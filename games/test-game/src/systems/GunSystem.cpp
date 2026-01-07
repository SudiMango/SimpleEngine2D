#include "systems/GunSystem.hpp"

namespace test_game::systems {

void GunSystem::init() {
    core::EventBus::getInstance().subscribe<events::MouseButtonDown>(this, [this](void *evt) {
        events::MouseButtonDown *_evt = (events::MouseButtonDown*)evt;

        if (_evt->button == SDL_BUTTON_LEFT) {
            isFiring = true;
        }
    });

    core::EventBus::getInstance().subscribe<events::CollisionEnter>(this, [this](void *evt) {
        events::CollisionEnter *_evt = (events::CollisionEnter*)evt;

        auto it1 = bullets.find(_evt->entity1);
        auto it2 = bullets.find(_evt->entity2);

        if (it1 != bullets.end() && it2 != bullets.end()) {
            em.deleteEntity(_evt->entity1);
            em.deleteEntity(_evt->entity2);
            bullets.erase(_evt->entity1);
            bullets.erase(_evt->entity2);
        } else if (it1 != bullets.end()) {
            em.deleteEntity(_evt->entity1);
            bullets.erase(_evt->entity1);
        } else if (it2 != bullets.end()) {
            em.deleteEntity(_evt->entity2);
            bullets.erase(_evt->entity2);
        }
    });
}

void GunSystem::update(float dt) {

    for (auto &pair : bullets) {
        pair.second += dt;

        if (pair.second >= MAX_ALIVE_TIME) {
            em.deleteEntity(pair.first);
            bullets.erase(pair.first);
        }
    }

    if (isFiring) {
        isFiring = false;

        components::TransformComponent *gunTransform = em.getComponent<components::TransformComponent>(attachedEntity);

        core::EntityId bullet = em.createEntity();
        components::TransformComponent *bulletTransform = new components::TransformComponent();
        bulletTransform->rotation = gunTransform->rotation;
        bulletTransform->scale = {50, 25};
        bulletTransform->position = {gunTransform->position.x + (gunTransform->scale.x/2) - (bulletTransform->scale.x/2), gunTransform->position.y + (gunTransform->scale.y/2) - (bulletTransform->scale.y/2)};
        bulletTransform->anchor = {(int)(gunTransform->anchor.x - (gunTransform->scale.x/2) + (bulletTransform->scale.x/2)), (int)(gunTransform->anchor.y - (gunTransform->scale.y/2) + (bulletTransform->scale.y/2))};
        components::MeshComponent *mesh = new components::MeshComponent();
        mesh->imagePath = (char*)"./games/test-game/assets/textures/bullet.bmp";
        components::RigidBodyComponent *rb = new components::RigidBodyComponent();
        rb->maxVelocity = 1000;
        glm::vec2 dir = {glm::cos(glm::radians(bulletTransform->rotation)), glm::sin(glm::radians(bulletTransform->rotation))};
        rb->velocity = rb->maxVelocity * dir;
        components::ColliderComponent *collider = new components::ColliderComponent();
        collider->showOutline = true;
        collider->isTrigger = true;
        collider->encompassTransform = true;
        collider->ignore.push_back(attachedEntity);
        collider->ignore.push_back(player);
        em.addComponent<components::TransformComponent>(bullet, bulletTransform);
        em.addComponent<components::MeshComponent>(bullet, mesh);
        em.addComponent<components::RigidBodyComponent>(bullet, rb);
        em.addComponent<components::ColliderComponent>(bullet, collider);

        bullets[bullet] = 0.0f;
    }
}

void GunSystem::clean() {
    core::EventBus::getInstance().unsubscribe(this);
}

}