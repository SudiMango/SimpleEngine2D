#include "TestScene.hpp"

namespace test_game::scenes {

void TestScene::setup() {
    core::EntityManager &em = core::EntityManager::getInstance();

    core::EntityId block = em.createEntity();
    components::TransformComponent *transform2 = new components::TransformComponent();
    transform2->position = {0, 0};
    transform2->scale = {200, 200};
    components::MeshComponent *mesh2 = new components::MeshComponent();
    mesh2->imagePath = (char *)"./games/test-game/assets/textures/achiles.bmp";
    em.addComponent<components::TransformComponent>(block, transform2);
    em.addComponent<components::MeshComponent>(block, mesh2);

    core::EventBus::getInstance().subscribe<events::InputBegan>([this](void *evt) {
        events::InputBegan *_evt = (events::InputBegan*)evt;

        if (_evt->keycode == SDLK_SPACE && !spacePressed) {
            spacePressed = true;
            std::cout << "Space pressed, switching scenes..." << std::endl;
            events::ChangeScene changeScene{1};
            core::EventBus::getInstance().publish<events::ChangeScene>(&changeScene);
        }
    });
}

}