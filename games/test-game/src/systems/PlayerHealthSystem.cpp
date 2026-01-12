#include "systems/PlayerHealthSystem.hpp"

namespace test_game::systems {

void PlayerHealthSystem::init() {
    core::EntityId bg = em.createEntity();
    simpleengine2d::components::TransformComponent *t0 = new simpleengine2d::components::TransformComponent();
    t0->scale = {healthBarMaxWidth, 50};
    t0->position = {config.window.width - 350, config.window.height - 80};
    simpleengine2d::components::PanelGuiComponent *bgPanel = new simpleengine2d::components::PanelGuiComponent();
    bgPanel->color = {189, 189, 189, 255};
    bgPanel->zIndex = 101;
    em.addComponent<simpleengine2d::components::TransformComponent>(bg, t0);
    em.addComponent<simpleengine2d::components::PanelGuiComponent>(bg, bgPanel);

    core::EntityId e = em.createEntity();
    simpleengine2d::components::TransformComponent *t1 = new simpleengine2d::components::TransformComponent();
    t1->scale = {healthBarMaxWidth, 50};
    simpleengine2d::components::PanelGuiComponent *healthPanel = new simpleengine2d::components::PanelGuiComponent();
    healthPanel->color = {0, 255, 0, 255};
    healthPanel->zIndex = 102;
    em.addComponent<simpleengine2d::components::TransformComponent>(e, t1);
    em.addComponent<simpleengine2d::components::PanelGuiComponent>(e, healthPanel);

    core::EntityId e2 = em.createEntity();
    simpleengine2d::components::TransformComponent *t2 = new simpleengine2d::components::TransformComponent();
    t2->scale = {150, 30};
    simpleengine2d::components::TextGuiComponent *healthText = new simpleengine2d::components::TextGuiComponent();
    healthText->text = "000/000";
    healthText->fontPath = (char*)"./games/test-game/assets/fonts/modeseven.ttf";
    healthText->zIndex = 103;
    em.addComponent<simpleengine2d::components::TransformComponent>(e2, t2);
    em.addComponent<simpleengine2d::components::TextGuiComponent>(e2, healthText);

    simpleengine2d::components::WeldComponent *weld = new simpleengine2d::components::WeldComponent();

    simpleengine2d::components::_welddata healthToBg;
    healthToBg.child = e;
    weld->welds.push_back(healthToBg);

    simpleengine2d::components::_welddata textToBg;
    textToBg.child = e2;
    textToBg.positionOffset = {t0->scale.x/2 - t2->scale.x/2, t0->scale.y/2 - t2->scale.y/2};
    weld->welds.push_back(textToBg);

    em.addComponent<simpleengine2d::components::WeldComponent>(bg, weld);

    healthbar = t1;
    this->healthText = healthText;
}

void PlayerHealthSystem::update(float dt) {
    if (em.hasComponent<test_game::components::HealthComponent>(player)) {
        auto *hComp = em.getComponent<test_game::components::HealthComponent>(player);
        if (hComp->currHealth > hComp->maxHealth) {
            hComp->currHealth = hComp->maxHealth;
        }

        std::string healthStr = std::to_string(static_cast<int>(std::round(hComp->currHealth))) + "/" + std::to_string(static_cast<int>(std::round(hComp->maxHealth)));
        if (healthText->text != healthStr) {
            SDL_DestroyTexture(healthText->texture);
            healthText->texture = nullptr;
            healthText->text = healthStr;
        }

        healthbar->scale.x = std::clamp((hComp->currHealth/hComp->maxHealth) * healthBarMaxWidth, 0.0f, healthBarMaxWidth);
    }
} 

}