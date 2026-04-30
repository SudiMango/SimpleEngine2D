#pragma once

#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <glm/glm.hpp>
#include "SimpleEngine2D/core/Engine.hpp"
#include "SimpleEngine2D/core/System.hpp"
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/core/ConfigManager.hpp"
#include "SimpleEngine2D/components/Camera.hpp"
#include "SimpleEngine2D/components/Mesh.hpp"
#include "SimpleEngine2D/components/Transform.hpp"
#include "SimpleEngine2D/components/Collider.hpp"
#include "SimpleEngine2D/components/Weld.hpp"
#include "SimpleEngine2D/components/TextGui.hpp"
#include "SimpleEngine2D/components/PanelGui.hpp"

namespace simpleengine2d::systems {

class Render : public core::System {

public:
    Render(SDL_Window *window, int index, Uint32 flags);

    void init() override;
    void update(float dt) override;
    void clean() override;

protected:
    SDL_Renderer *renderer;
    core::EntityManager &em = core::EntityManager::getInstance();
    core::Config &config = core::ConfigManager::getInstance().getConfig();

    void renderScene(float dt);

    std::vector<core::EntityId> buildRenderList();
    void renderMeshFor(core::EntityId entity, float dt);
    void renderColliderFor(core::EntityId entity);
    void renderGuiFor(core::EntityId entity);
    void loadSurfaces(core::EntityId entity);
};

}
