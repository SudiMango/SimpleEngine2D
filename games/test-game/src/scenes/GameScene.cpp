#include "scenes/GameScene.hpp"

namespace test_game::scenes {

void GameScene::setup() {
    // Set up everything here

    core::EntityManager &em = core::EntityManager::getInstance();

    // Create player
    core::EntityId player = em.createEntity();
    components::TransformComponent *transform = new components::TransformComponent();
    transform->position = {(1920/2) - 100, (1088/2) - 100};
    transform->scale = {200, 200};
    transform->anchor = {(int)(transform->scale.x/2), (int)(transform->scale.y/2)};
    // transform->anchor = {0, 0};
    components::MeshComponent *mesh = new components::MeshComponent();
    mesh->imagePath = (char *)"./games/test-game/assets/textures/raeed.bmp";
    components::RigidBodyComponent *rb = new components::RigidBodyComponent();
    rb->maxVelocity = 500;
    rb->useGravity = true;
    components::ColliderComponent *collider = new components::ColliderComponent();
    collider->showOutline = true;
    collider->encompassTransform = true;
    em.addComponent<components::TransformComponent>(player, transform);
    em.addComponent<components::MeshComponent>(player, mesh);
    em.addComponent<components::RigidBodyComponent>(player, rb);
    em.addComponent<components::ColliderComponent>(player, collider);

    // Create weapon
    core::EntityId pistol = em.createEntity();
    components::TransformComponent *p_transform = new components::TransformComponent();
    p_transform->scale = {100, 75};
    // p_transform->anchor = {(int)((p_transform->scale.x/2) - (transform->scale.x/2)), (int)(p_transform->scale.y/2)};
    p_transform->anchor = {(int)(p_transform->scale.x/2), (int)(p_transform->scale.y/2)};
    // p_transform->anchor = {0, 0};
    components::MeshComponent *p_mesh = new components::MeshComponent();
    p_mesh->imagePath = (char*)"./games/test-game/assets/textures/pistol.bmp";
    p_mesh->zIndex = 100;
    em.addComponent<components::TransformComponent>(pistol, p_transform);
    em.addComponent<components::MeshComponent>(pistol, p_mesh);

    // Create weld constraint
    components::WeldComponent *weld = new components::WeldComponent();
    components::_welddata pistolWeldData;
    pistolWeldData.child = pistol;
    pistolWeldData.followRotation = false;
    pistolWeldData.positionOffset = {((transform->scale.x - p_transform->scale.x)/2) +  (transform->scale.x/2), (transform->scale.y - p_transform->scale.y)/2};
    // pistolWeldData.positionOffset = {(transform->scale.x - p_transform->scale.x)/2, (transform->scale.y - p_transform->scale.y)/2};
    // pistolWeldData.positionOffset = {0, 0};
    weld->welds.push_back(pistolWeldData);
    em.addComponent<components::WeldComponent>(player, weld);

    // Environment objects

    core::EntityId block = em.createEntity();
    components::TransformComponent *transform2 = new components::TransformComponent();
    transform2->position = {500, 500};
    transform2->scale = {200, 200};
    components::MeshComponent *mesh2 = new components::MeshComponent();
    mesh2->imagePath = (char *)"./games/test-game/assets/textures/sudi.bmp";
    components::ColliderComponent *collider2 = new components::ColliderComponent();
    collider2->showOutline = true;
    em.addComponent<components::TransformComponent>(block, transform2);
    em.addComponent<components::MeshComponent>(block, mesh2);
    em.addComponent<components::ColliderComponent>(block, collider2);

    core::EntityId floor = em.createEntity();
    components::TransformComponent *transform3 = new components::TransformComponent();
    transform3->position = {500, 800};
    transform3->scale = {1000, 200};
    components::MeshComponent *mesh3 = new components::MeshComponent();
    mesh3->imagePath = (char *)"./games/test-game/assets/textures/achiles.bmp";
    components::ColliderComponent *collider3 = new components::ColliderComponent();
    em.addComponent<components::TransformComponent>(floor, transform3);
    em.addComponent<components::MeshComponent>(floor, mesh3);
    em.addComponent<components::ColliderComponent>(floor, collider3);
    core::TagManager::getInstance().createTag("ground");
    core::TagManager::getInstance().addTag(floor, "ground");

    core::EntityId wall = em.createEntity();
    components::TransformComponent *wt = new components::TransformComponent();
    wt->position = {1700, 0};
    wt->scale = {100, 1000};
    components::MeshComponent *wm = new components::MeshComponent();
    wm->imagePath = (char *)"./games/test-game/assets/textures/malir.bmp";
    components::ColliderComponent *wc = new components::ColliderComponent();
    em.addComponent<components::TransformComponent>(wall, wt);
    em.addComponent<components::MeshComponent>(wall, wm);
    em.addComponent<components::ColliderComponent>(wall, wc);

    // Gui stuff
    core::EntityId t = em.createEntity();
    components::TextGuiComponent *textGui = new components::TextGuiComponent();
    textGui->text = (char*)"Hello world!";
    textGui->fontPath = (char*)"../game/assets/fonts/modeseven.ttf";
    textGui->textSize = 50;
    textGui->textColor = {0, 255, 0, 255};
    em.addComponent<components::TextGuiComponent>(t, textGui);

    core::EntityId p = em.createEntity();
    components::PanelGuiComponent *panelGui = new components::PanelGuiComponent();
    panelGui->scale = {100, 100};
    panelGui->position = {100, 100};
    panelGui->color = {0, 0, 255, 255};
    em.addComponent<components::PanelGuiComponent>(p, panelGui);

    // Add custom systems
    core::Engine::getInstance().addSystem(new test_game::systems::SidewaysMovementSystem(player));
    core::Engine::getInstance().addSystem(new test_game::systems::SidewaysLookingSystem(player, pistol));
    core::Engine::getInstance().addSystem(new test_game::systems::GunSystem(pistol, player));
}

}