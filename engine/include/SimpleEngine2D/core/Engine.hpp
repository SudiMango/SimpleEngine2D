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
    static Engine &getInstance();
    
    void init(const char* title, int width, int height, bool fullscreen);
    void run();

    void addSystem(System *system);

private:
    Engine() = default;
    ~Engine() = default;

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    bool running = false;
    SDL_Window *window;

    std::vector<System*> systems;

    float dt = 0.0f;
    std::chrono::steady_clock::time_point lastFrameTime;

    float acc = 0.0f;
    const float FIXED_UPDATE_DELAY = 0.02f;

    EntityManager &em = EntityManager::getInstance();

    void clean();
};

}
