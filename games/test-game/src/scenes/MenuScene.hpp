#pragma once

#include <iostream>
#include "SimpleEngine2D/core/Scene.hpp"
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/core/EventBus.hpp"
#include "SimpleEngine2D/core/SceneManager.hpp"
#include "SimpleEngine2D/components/Transform.hpp"
#include "SimpleEngine2D/components/Mesh.hpp"
#include "SimpleEngine2D/components/PanelGui.hpp"
#include "SimpleEngine2D/components/TextGui.hpp"
#include "SimpleEngine2D/components/Weld.hpp"
#include "SimpleEngine2D/events/InputBegan.hpp"
#include "SimpleEngine2D/events/RequestSceneChange.hpp"

using namespace simpleengine2d;

namespace test_game::scenes {

class MenuScene : public core::Scene {

public:
    void setup() override;
    void clean() override;

};

}