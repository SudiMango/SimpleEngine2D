#pragma once

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <chrono>
#include "SimpleEngine2D/core/System.hpp"
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/systems/Input.hpp"
#include "SimpleEngine2D/systems/Render.hpp"
#include "SimpleEngine2D/systems/Collision.hpp"
#include "SimpleEngine2D/systems/Physics.hpp"

namespace simpleengine2d::core {

class Engine {

public:
    Engine() = default;
    ~Engine() = default;
    
    void init(const char* title, int width, int height, bool fullscreen);
    void run();

    void addSystem(System *system);

    EntityManager &getEntityManager() {
        return EntityManager::getInstance();
    }

private:
    bool running = false;
    SDL_Window *window;

    systems::Input *inputSystem;
    systems::Render *renderSystem;
    std::vector<System*> systems;

    float dt = 0.0f;
    std::chrono::steady_clock::time_point lastFrameTime;

    void clean();
};

}
