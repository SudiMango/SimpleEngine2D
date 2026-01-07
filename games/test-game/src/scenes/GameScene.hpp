#pragma once

#include "SimpleEngine2D/core/Engine.hpp"
#include "SimpleEngine2D/core/Scene.hpp"
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/core/TagManager.hpp"
#include "SimpleEngine2D/core/EventBus.hpp"

#include "systems/SidewaysLookingSystem.hpp"
#include "systems/SidewaysMovementSystem.hpp"
#include "systems/GunSystem.hpp"

using namespace simpleengine2d;

namespace test_game::scenes {

class GameScene : public core::Scene {

public:
    void setup() override;

private:


};

}