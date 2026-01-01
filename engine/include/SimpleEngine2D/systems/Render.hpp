#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <glm/glm.hpp>
#include "SimpleEngine2D/core/System.hpp"
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/components/Camera.hpp"
#include "SimpleEngine2D/components/Mesh.hpp"
#include "SimpleEngine2D/components/Transform.hpp"
#include "SimpleEngine2D/components/Collider.hpp"
#include "SimpleEngine2D/components/Weld.hpp"

namespace simpleengine2d::systems {

class Render : public core::System {

public:
    Render(SDL_Window *window, int index, Uint32 flags);

    void init() override;
    void update(float dt) override;
    void clean() override;

private:
    SDL_Renderer *renderer;
    core::EntityManager &em = core::EntityManager::getInstance();

};

}
