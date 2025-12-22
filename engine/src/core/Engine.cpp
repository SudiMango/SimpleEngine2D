#include "SimpleEngine2D/core/Engine.hpp"
#include "SimpleEngine2D/components/Camera.hpp"

namespace simpleengine2d::core {

void Engine::init(const char* title, int width, int height, bool isFullscreen) {
    std::cout << "Initializing engine..." << std::endl;

    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, (isFullscreen) ? SDL_WINDOW_FULLSCREEN : 0);

    renderSystem = new simpleengine2d::systems::Render(window, -1, 0);
    inputSystem = new simpleengine2d::systems::Input(running);

    addSystem(inputSystem);
    addSystem(renderSystem);
    addSystem(new simpleengine2d::systems::Collision());
    addSystem(new simpleengine2d::systems::Physics());

    EntityId camera = EntityManager::getInstance().createEntity();
    simpleengine2d::components::Camera *cam = new simpleengine2d::components::Camera{};
    cam->position = {0, 0};
    cam->zoom = 1;
    cam->follow = 1;
    EntityManager::getInstance().addComponent<simpleengine2d::components::Camera>(camera, cam);

    running = true;

    lastFrameTime = std::chrono::steady_clock::now();
    std::cout << "Engine initialized!" << std::endl;
}

void Engine::run() {

    for (auto s : systems) {
        s->init();
    }

    while (running) {
        
        auto currentTime = std::chrono::steady_clock::now();
        dt = std::chrono::duration<float>(currentTime - lastFrameTime).count();
        lastFrameTime = currentTime;

        for (auto s : systems) {
            s->update(dt);
        }
    }
    
    clean();
}

void Engine::clean() {
    for (auto s : systems) {
        s->clean();
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "Cleaned, exiting..." << std::endl;
}

void Engine::addSystem(System *system) {
    systems.push_back(system);
}

}
