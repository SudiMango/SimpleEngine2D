#pragma once

#include <string>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "SimpleEngine2D/core/System.hpp"
#include "SimpleEngine2D/core/EventBus.hpp"
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/core/ConfigManager.hpp"

#include "SimpleEngine2D/components/Transform.hpp"
#include "SimpleEngine2D/components/Weld.hpp"
#include "SimpleEngine2D/components/TextGui.hpp"
#include "SimpleEngine2D/components/PanelGui.hpp"

#include "SimpleEngine2D/events/CollisionEnter.hpp"

#include "components/HealthComponent.hpp"
#include "components/DamageComponent.hpp"

using namespace simpleengine2d;

namespace test_game::systems {

class PlayerHealthSystem : public core::System {

public:
    PlayerHealthSystem(core::EntityId player) : player(player) {};

    void init() override;
    void update(float dt) override;
    void clean() override {};

private:
    core::EntityId player;

    core::EntityManager &em = core::EntityManager::getInstance();
    core::Config &config = core::ConfigManager::getInstance().getConfig();

    float healthBarMaxWidth = 300.0f;
    simpleengine2d::components::TransformComponent *healthbar;
    simpleengine2d::components::TextGuiComponent *healthText;

};

}