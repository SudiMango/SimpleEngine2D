#include "factories/EnemyFactory.hpp"

namespace test_game::factories {

simpleengine2d::core::EntityId EnemyFactory::create(glm::vec2 position) {
    auto enemy = em.createEntity();
    auto *e_transform = new simpleengine2d::components::TransformComponent();
    e_transform->position = position;
    e_transform->scale = {100, 100};
    auto *e_mesh = new simpleengine2d::components::MeshComponent();
    e_mesh->imagePath = (char *)"./games/test-game/assets/textures/enemy.bmp";
    auto *e_rb = new simpleengine2d::components::RigidBodyComponent();
    e_rb->maxVelocity = 200;
    e_rb->useGravity = true;
    auto *e_collider = new simpleengine2d::components::ColliderComponent();
    e_collider->encompassTransform = true;
    auto *e_hComp = new components::HealthComponent();
    e_hComp->maxHealth = 50;
    e_hComp->currHealth = e_hComp->maxHealth;
    auto *ec = new components::EnemyComponent();
    ec->patrolDistance = 200;
    ec->playerDetectionRange = 400;
    em.addComponent<simpleengine2d::components::TransformComponent>(enemy, e_transform);
    em.addComponent<simpleengine2d::components::MeshComponent>(enemy, e_mesh);
    em.addComponent<simpleengine2d::components::RigidBodyComponent>(enemy, e_rb);
    em.addComponent<simpleengine2d::components::ColliderComponent>(enemy, e_collider);
    em.addComponent<components::HealthComponent>(enemy, e_hComp);
    em.addComponent<components::EnemyComponent>(enemy, ec);

    auto e_pistol = em.createEntity();
    auto *ep_transform = new simpleengine2d::components::TransformComponent();
    ep_transform->scale = {60, 40};
    ep_transform->anchor = {(int)(ep_transform->scale.x/2), (int)(ep_transform->scale.y/2)};
    auto *ep_mesh = new simpleengine2d::components::MeshComponent();
    ep_mesh->imagePath = (char*)"./games/test-game/assets/textures/pistol.bmp";
    ep_mesh->zIndex = 100;
    auto *e_gun = new components::GunComponent();
    e_gun->damage = 10;
    e_gun->timeBetweenFire = 0.5;
    e_gun->timeSinceLastFire = e_gun->timeBetweenFire;
    em.addComponent<simpleengine2d::components::TransformComponent>(e_pistol, ep_transform);
    em.addComponent<simpleengine2d::components::MeshComponent>(e_pistol, ep_mesh);
    em.addComponent<components::GunComponent>(e_pistol, e_gun);

    auto *e_weld = new simpleengine2d::components::WeldComponent();
    simpleengine2d::components::_welddata e_pistolWeldData;
    e_pistolWeldData.child = e_pistol;
    e_pistolWeldData.followRotation = false;
    e_pistolWeldData.positionOffset = {((e_transform->scale.x - ep_transform->scale.x)/2) +  (e_transform->scale.x/2), (e_transform->scale.y - ep_transform->scale.y)/2 + 10};
    e_weld->welds.push_back(e_pistolWeldData);
    em.addComponent<simpleengine2d::components::WeldComponent>(enemy, e_weld);

    auto e_bulletSpawn = em.createEntity();
    auto *eb_transform = new simpleengine2d::components::TransformComponent();
    eb_transform->anchor = {(int)(eb_transform->scale.x - ep_transform->scale.x/2), (int)(eb_transform->scale.y/2 + 10)};
    em.addComponent<simpleengine2d::components::TransformComponent>(e_bulletSpawn, eb_transform);

    auto *e_weld2 = new simpleengine2d::components::WeldComponent();
    simpleengine2d::components::_welddata e_bulletWeldData;
    e_bulletWeldData.child = e_bulletSpawn;
    e_bulletWeldData.followRotation = true;
    e_bulletWeldData.positionOffset = {ep_transform->scale.x - eb_transform->scale.x, ep_transform->scale.y/2 - eb_transform->scale.y/2 - 10};
    e_weld2->welds.push_back(e_bulletWeldData);
    em.addComponent<simpleengine2d::components::WeldComponent>(e_pistol, e_weld2);

    return enemy;
}

}