#include "scenes/GameScene.hpp"

namespace test_game::scenes {

void GameScene::setup() {
    config.render.backgroundColor = {201, 145, 103, 255};

    auto *cam = core::Engine::getInstance().getCamera();
    cam->follow = 1;
    cam->shouldLerp = true;
    cam->smoothingFactor = 4;
    cam->followOffset = {100, -200};

    // Create player
    auto player = em.createEntity();
    auto *transform = new simpleengine2d::components::TransformComponent();
    transform->position = {(1920/2) - 50, (1088/2) - 50};
    transform->scale = {100, 100};
    transform->anchor = {(int)(transform->scale.x/2), (int)(transform->scale.y/2)};
    auto *mesh = new simpleengine2d::components::MeshComponent();
    mesh->imagePath = (char *)"./games/test-game/assets/textures/player.bmp";
    auto *rb = new simpleengine2d::components::RigidBodyComponent();
    rb->maxVelocity = 500;
    rb->useGravity = true;
    auto *collider = new simpleengine2d::components::ColliderComponent();
    collider->encompassTransform = true;
    test_game::components::HealthComponent *hComp = new test_game::components::HealthComponent();
    em.addComponent<simpleengine2d::components::TransformComponent>(player, transform);
    em.addComponent<simpleengine2d::components::MeshComponent>(player, mesh);
    em.addComponent<simpleengine2d::components::RigidBodyComponent>(player, rb);
    em.addComponent<simpleengine2d::components::ColliderComponent>(player, collider);
    em.addComponent<test_game::components::HealthComponent>(player, hComp);

    // Create weapon
    auto pistol = em.createEntity();
    auto *p_transform = new simpleengine2d::components::TransformComponent();
    p_transform->scale = {60, 40};
    p_transform->anchor = {(int)(p_transform->scale.x/2), (int)(p_transform->scale.y/2)};
    auto *p_mesh = new simpleengine2d::components::MeshComponent();
    p_mesh->imagePath = (char*)"./games/test-game/assets/textures/pistol.bmp";
    p_mesh->zIndex = 100;
    components::GunComponent *gun = new components::GunComponent();
    gun->damage = 20;
    gun->timeBetweenFire = 0.25;
    gun->timeSinceLastFire = gun->timeBetweenFire;
    em.addComponent<simpleengine2d::components::TransformComponent>(pistol, p_transform);
    em.addComponent<simpleengine2d::components::MeshComponent>(pistol, p_mesh);
    em.addComponent<components::GunComponent>(pistol, gun);

    // Create weld constraint
    auto *weld = new simpleengine2d::components::WeldComponent();
    simpleengine2d::components::_welddata pistolWeldData;
    pistolWeldData.child = pistol;
    pistolWeldData.followRotation = false;
    pistolWeldData.positionOffset = {((transform->scale.x - p_transform->scale.x)/2) +  (transform->scale.x/2), (transform->scale.y - p_transform->scale.y)/2 + 10};
    weld->welds.push_back(pistolWeldData);
    em.addComponent<simpleengine2d::components::WeldComponent>(player, weld);

    auto bulletSpawn = em.createEntity();
    auto *b_transform = new simpleengine2d::components::TransformComponent();
    b_transform->anchor = {(int)(b_transform->scale.x - p_transform->scale.x/2), (int)(b_transform->scale.y/2 + 10)};
    em.addComponent<simpleengine2d::components::TransformComponent>(bulletSpawn, b_transform);

    auto *weld2 = new simpleengine2d::components::WeldComponent();
    simpleengine2d::components::_welddata bulletWeldData;
    bulletWeldData.child = bulletSpawn;
    bulletWeldData.followRotation = true;
    bulletWeldData.positionOffset = {p_transform->scale.x - b_transform->scale.x, p_transform->scale.y/2 - b_transform->scale.y/2 - 10};
    weld2->welds.push_back(bulletWeldData);
    em.addComponent<simpleengine2d::components::WeldComponent>(pistol, weld2);

    // Create enemy
    factories::EnemyFactory enemyFactory;
    auto enemy = enemyFactory.create({2000, (1088/2) - 50});
    auto enemy2 = enemyFactory.create({1200, 850});

    // Environment objects
    core::TagManager::getInstance().createTag("ground");

    auto floor = em.createEntity();
    auto *transform3 = new simpleengine2d::components::TransformComponent();
    transform3->position = {500, 800};
    transform3->scale = {1000, 100};
    auto *mesh3 = new simpleengine2d::components::MeshComponent();
    mesh3->color = {20, 20, 20, 255};
    auto *collider3 = new simpleengine2d::components::ColliderComponent();
    em.addComponent<simpleengine2d::components::TransformComponent>(floor, transform3);
    em.addComponent<simpleengine2d::components::MeshComponent>(floor, mesh3);
    em.addComponent<simpleengine2d::components::ColliderComponent>(floor, collider3);
    core::TagManager::getInstance().createTag("ground");
    core::TagManager::getInstance().addTag(floor, "ground");

    auto floor2 = em.createEntity();
    auto *t4 = new simpleengine2d::components::TransformComponent();
    t4->position = {1700, 800};
    t4->scale = {1000, 100};
    auto *m4 = new simpleengine2d::components::MeshComponent();
    m4->color = {20, 20, 20, 255};
    auto *c4 = new simpleengine2d::components::ColliderComponent();
    em.addComponent<simpleengine2d::components::TransformComponent>(floor2, t4);
    em.addComponent<simpleengine2d::components::MeshComponent>(floor2, m4);
    em.addComponent<simpleengine2d::components::ColliderComponent>(floor2, c4);
    simpleengine2d::core::TagManager::getInstance().addTag(floor2, "ground");

    auto floor3 = em.createEntity();
    auto *t5 = new simpleengine2d::components::TransformComponent();
    t5->position = {1000, 1100};
    t5->scale = {1000, 100};
    auto *m5 = new simpleengine2d::components::MeshComponent();
    m5->color = {20, 20, 20, 255};
    auto *c5 = new simpleengine2d::components::ColliderComponent();
    em.addComponent<simpleengine2d::components::TransformComponent>(floor3, t5);
    em.addComponent<simpleengine2d::components::MeshComponent>(floor3, m5);
    em.addComponent<simpleengine2d::components::ColliderComponent>(floor3, c5);
    simpleengine2d::core::TagManager::getInstance().addTag(floor3, "ground");

    // Add custom systems
    core::Engine::getInstance().addSystem(new test_game::systems::SidewaysMovementSystem(player));
    core::Engine::getInstance().addSystem(new test_game::systems::SidewaysLookingSystem(player, pistol));
    core::Engine::getInstance().addSystem(new test_game::systems::GunSystem(pistol, player));
    core::Engine::getInstance().addSystem(new test_game::systems::PlayerHealthSystem(player));
    core::Engine::getInstance().addSystem(new test_game::systems::EnemySystem(player, {enemy, enemy2}));

    core::EventBus::getInstance().subscribe<simpleengine2d::events::InputBegan>(this, [this](void *evt) {
        auto *_evt = (simpleengine2d::events::InputBegan*)evt;

        if (_evt->keycode == SDLK_r) {
            std::cout << "R pressed, switching scenes..." << std::endl;
            simpleengine2d::events::RequestSceneChange rsc{0};
            core::EventBus::getInstance().publish<simpleengine2d::events::RequestSceneChange>(&rsc);
        }
    });
}

void GameScene::clean() {
    core::EventBus::getInstance().unsubscribe(this);
}

}
