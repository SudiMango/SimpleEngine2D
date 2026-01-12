#include "MenuScene.hpp"

namespace test_game::scenes {

void MenuScene::setup() {
    config.render.backgroundColor = {0, 0, 0, 255};

    core::EntityId p = em.createEntity();
    simpleengine2d::components::PanelGuiComponent *panel = new simpleengine2d::components::PanelGuiComponent();
    panel->color = {20, 20, 20, 255};
    simpleengine2d::components::TransformComponent *t1 = new simpleengine2d::components::TransformComponent();
    t1->scale = {config.window.width, config.window.height};
    t1->position = {config.window.width/2 - t1->scale.x/2, config.window.height/2 - t1->scale.y/2};
    em.addComponent<simpleengine2d::components::PanelGuiComponent>(p, panel);
    em.addComponent<simpleengine2d::components::TransformComponent>(p, t1);

    core::EntityId t = em.createEntity();
    simpleengine2d::components::TextGuiComponent *textGui = new simpleengine2d::components::TextGuiComponent();
    textGui->text = "Risk of Mango";
    textGui->fontPath = (char*)"./games/test-game/assets/fonts/modeseven.ttf";
    simpleengine2d::components::TransformComponent *t2 = new simpleengine2d::components::TransformComponent();
    t2->scale = {800, 80};
    em.addComponent<simpleengine2d::components::TextGuiComponent>(t, textGui);
    em.addComponent<simpleengine2d::components::TransformComponent>(t, t2);

    core::EntityId text2 = em.createEntity();
    simpleengine2d::components::TextGuiComponent *textGui2 = new simpleengine2d::components::TextGuiComponent();
    textGui2->text = "Press SPACE to start";
    textGui2->fontPath = (char*)"./games/test-game/assets/fonts/modeseven.ttf";
    simpleengine2d::components::TransformComponent *t3 = new simpleengine2d::components::TransformComponent();
    t3->scale = {400, 40};
    em.addComponent<simpleengine2d::components::TextGuiComponent>(text2, textGui2);
    em.addComponent<simpleengine2d::components::TransformComponent>(text2, t3);

    simpleengine2d::components::WeldComponent *weld = new simpleengine2d::components::WeldComponent();
    simpleengine2d::components::_welddata wd;
    wd.child = t;
    wd.positionOffset = {t1->scale.x/2 - t2->scale.x/2, t1->scale.y/2 - t2->scale.y/2 - 40};
    weld->welds.push_back(wd);
    simpleengine2d::components::_welddata w2;
    w2.child = text2;
    w2.positionOffset = {t1->scale.x/2 - t3->scale.x/2, t1->scale.y/2 - t3->scale.y/2 + 60};
    weld->welds.push_back(w2);
    em.addComponent<simpleengine2d::components::WeldComponent>(p, weld);

    core::EventBus::getInstance().subscribe<events::InputBegan>(this, [this](void *evt) {
        events::InputBegan *_evt = (events::InputBegan*)evt;

        if (_evt->keycode == SDLK_SPACE) {
            std::cout << "Space pressed, switching scenes..." << std::endl;
            events::RequestSceneChange rsc{1};
            core::EventBus::getInstance().publish<events::RequestSceneChange>(&rsc);
        }
    });
}

void MenuScene::clean() {
    core::EventBus::getInstance().unsubscribe(this);
}

}