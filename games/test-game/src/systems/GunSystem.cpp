#include "systems/GunSystem.hpp"

namespace test_game::systems {

/**
 * Public functions
 */

void GunSystem::init() {
    core::EventBus::getInstance().subscribe<simpleengine2d::events::MouseButtonDown>(this, [this](void *evt) {
        simpleengine2d::events::MouseButtonDown *_evt = (simpleengine2d::events::MouseButtonDown*)evt;

        if (_evt->button == SDL_BUTTON_LEFT) {
            isFiring = true;
        }
    });

    core::EventBus::getInstance().subscribe<simpleengine2d::events::CollisionEnter>(this, [this](void *evt) {
        simpleengine2d::events::CollisionEnter *_evt = (simpleengine2d::events::CollisionEnter*)evt;

        auto it1 = bullets.find(_evt->entity1);
        auto it2 = bullets.find(_evt->entity2);

        if (it1 != bullets.end() && it2 != bullets.end()) {
            
        } else if (it1 != bullets.end()) {
            if (em.hasComponent<components::HealthComponent>(_evt->entity2)) {
                auto *dmg = em.getComponent<components::DamageComponent>(_evt->entity1);
                auto *h = em.getComponent<components::HealthComponent>(_evt->entity2);

                h->currHealth -= dmg->damage;
            }

            em.deleteEntity(_evt->entity1);
            bullets.erase(_evt->entity1);
        } else if (it2 != bullets.end()) {
            if (em.hasComponent<components::HealthComponent>(_evt->entity1)) {
                auto *dmg = em.getComponent<components::DamageComponent>(_evt->entity2);
                auto *h = em.getComponent<components::HealthComponent>(_evt->entity1);

                h->currHealth -= dmg->damage;
            }

            em.deleteEntity(_evt->entity2);
            bullets.erase(_evt->entity2);
        }
    });

    core::EventBus::getInstance().subscribe<events::AddAmmo>(this, [this](void *evt) {
        auto *_evt = (events::AddAmmo*)evt;
        numBullets += _evt->ammo;
    });

    auto *weld = em.getComponent<simpleengine2d::components::WeldComponent>(attachedEntity);
    bulletSpawn = em.getComponent<simpleengine2d::components::TransformComponent>(weld->welds[0].child);

    auto gunGui = em.createEntity();
    auto *t = new simpleengine2d::components::TransformComponent();
    t->position = {30, 950};
    t->scale = {100, 100};
    auto *bulletText = new simpleengine2d::components::TextGuiComponent();
    bulletText->fontPath = (char*)"./games/test-game/assets/fonts/modeseven.ttf";
    bulletText->text = std::to_string(numBullets);
    em.addComponent<simpleengine2d::components::TransformComponent>(gunGui, t);
    em.addComponent<simpleengine2d::components::TextGuiComponent>(gunGui, bulletText);
    this->bulletText = bulletText;
}

void GunSystem::update(float dt) {

    for (auto &pair : bullets) {
        pair.second += dt;

        if (pair.second >= MAX_ALIVE_TIME) {
            em.deleteEntity(pair.first);
            bullets.erase(pair.first);
        }
    }

    auto *gunComp = em.getComponent<components::GunComponent>(attachedEntity);
    if (gunComp->timeSinceLastFire >= gunComp->timeBetweenFire && isFiring && numBullets > 0) {
        isFiring = false;
        createBullet();
        gunComp->timeSinceLastFire = 0;
        numBullets--;
    }

    if (bulletText->text != std::to_string(numBullets)) {
        SDL_DestroyTexture(bulletText->texture);
        bulletText->texture = nullptr;
        bulletText->text = std::to_string(numBullets);
    }

    isFiring = false;
    gunComp->timeSinceLastFire += dt;
}

void GunSystem::clean() {
    core::EventBus::getInstance().unsubscribe(this);
}

/**
 * Private functions
 */

void GunSystem::createBullet() {
    auto *gunComp = em.getComponent<components::GunComponent>(attachedEntity);

    auto bullet = em.createEntity();
    auto *bulletTransform = new simpleengine2d::components::TransformComponent();
    bulletTransform->rotation = bulletSpawn->rotation;
    bulletTransform->scale = {25, 4};
    bulletTransform->position = {bulletSpawn->position.x + (bulletSpawn->scale.x/2) - (bulletTransform->scale.x/2), bulletSpawn->position.y + (bulletSpawn->scale.y/2) - (bulletTransform->scale.y/2)};
    bulletTransform->anchor = {(int)(bulletSpawn->anchor.x - (bulletSpawn->scale.x/2) + (bulletTransform->scale.x/2)), (int)(bulletSpawn->anchor.y - (bulletSpawn->scale.y/2) + (bulletTransform->scale.y/2))};
    auto *mesh = new simpleengine2d::components::MeshComponent();
    mesh->imagePath = (char*)"./games/test-game/assets/textures/bullet.bmp";
    mesh->zIndex = 5;
    auto *rb = new simpleengine2d::components::RigidBodyComponent();
    rb->maxVelocity = 1000;
    glm::vec2 dir = {glm::cos(glm::radians(bulletTransform->rotation)), glm::sin(glm::radians(bulletTransform->rotation))};
    rb->velocity = rb->maxVelocity * dir;
    auto *collider = new simpleengine2d::components::ColliderComponent();
    collider->isTrigger = true;
    collider->encompassTransform = true;
    collider->ignore.push_back(attachedEntity);
    collider->ignore.push_back(player);
    auto *dmg = new components::DamageComponent();
    dmg->damage = gunComp->damage;
    em.addComponent<simpleengine2d::components::TransformComponent>(bullet, bulletTransform);
    em.addComponent<simpleengine2d::components::MeshComponent>(bullet, mesh);
    em.addComponent<simpleengine2d::components::RigidBodyComponent>(bullet, rb);
    em.addComponent<simpleengine2d::components::ColliderComponent>(bullet, collider);
    em.addComponent<components::DamageComponent>(bullet, dmg);

    bullets[bullet] = 0.0f;
}

}