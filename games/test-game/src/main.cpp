#include "SimpleEngine2D/core/Engine.hpp"
#include "SimpleEngine2D/core/SceneManager.hpp"
#include "scenes/MenuScene.hpp"
#include "scenes/GameScene.hpp"

using namespace simpleengine2d;

int main(int argc, char *argv[]) {
    core::Scene *menuScene = new test_game::scenes::MenuScene();
    core::Scene *gameScene = new test_game::scenes::GameScene();
    core::SceneManager::getInstance().addScene(menuScene);
    core::SceneManager::getInstance().addScene(gameScene);

    core::Engine &engine = core::Engine::getInstance();
    engine.init(1);
    engine.run();

    return 0;
}