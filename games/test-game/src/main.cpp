#include "SimpleEngine2D/core/Engine.hpp"
#include "SimpleEngine2D/core/SceneManager.hpp"
#include "scenes/TestScene.hpp"
#include "scenes/GameScene.hpp"

using namespace simpleengine2d;

char *title = (char*)"SimpleEngine";
int screenWidth = 1920;
int screenHeight = 1088;
bool isFullscreen = false;

int main(int argc, char *argv[]) {
    core::Engine &engine = core::Engine::getInstance();
    engine.init(title, screenWidth, screenHeight, isFullscreen);

    core::Scene *testScene = new test_game::scenes::TestScene();
    core::Scene *gameScene = new test_game::scenes::GameScene();
    core::SceneManager::getInstance().addScene(testScene);
    core::SceneManager::getInstance().addScene(gameScene);

    core::SceneManager::getInstance().init(0);

    engine.run();

    return 0;
}