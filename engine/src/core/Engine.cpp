#include "SimpleEngine2D/core/Engine.hpp"

namespace simpleengine2d::core {

/**
 * Public functions
 */

void Engine::init(int sceneIndex) {
    std::cout << "Initializing engine..." << std::endl;

    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
    window = SDL_CreateWindow(config.window.title, 
                            SDL_WINDOWPOS_CENTERED, 
                            SDL_WINDOWPOS_CENTERED, 
                            config.window.width, 
                            config.window.height, 
                            (config.window.fullscreen) ? SDL_WINDOW_FULLSCREEN : 0);

    nextSceneIndex = sceneIndex;
    this->setup();

    running = true;
    lastFrameTime = std::chrono::steady_clock::now();
    core::EventBus::getInstance().subscribe<events::RequestSceneChange>(this, [this](void *evt){
        if (!sceneChangeQueued) {
            this->onChangeSceneRequest(static_cast<events::RequestSceneChange*>(evt)->sceneIndex);
        }
    });

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

        fixedUpdateAcc += dt;
        if (fixedUpdateAcc >= FIXED_UPDATE_DELAY) {
            for (auto s : systems) {
                s->fixedUpdate(FIXED_UPDATE_DELAY);
            }
            fixedUpdateAcc -= FIXED_UPDATE_DELAY;
        }

        if (sceneChangeQueued) {
            this->setup();
        }
    }
    
    this->clean();
}

/**
 * Private functions
 */

void Engine::setup() {
    // Clear old systems
    for (auto s : systems) {
        s->clean();
        delete s;
    }
    systems.clear();
    em.reset();
    TagManager::getInstance().reset();

    // Re-add core systems
    // addSystem(new AudioSystem());
    addSystem(new systems::Input(running));
    addSystem(new systems::Physics());
    addSystem(new systems::Collision());
    addSystem(new systems::Render(window, -1, 0));

    // Add camera
    EntityId camera = em.createEntity();
    components::Camera *cam = new components::Camera{};
    cam->position = {0, 0};
    em.addComponent<components::Camera>(camera, cam);

    // Set up next scene
    SceneManager::getInstance().switchScene(nextSceneIndex);

    // Initialize all new systems
    for (auto s : systems) {
        s->init();
    }

    // Cleanup
    nextSceneIndex = -1;
    sceneChangeQueued = false;
    std::cout << "Changed scenes!" << std::endl;
}

void Engine::clean() {
    for (auto s : systems) {
        s->clean();
    }
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    std::cout << "Cleaned, exiting..." << std::endl;
}

void Engine::onChangeSceneRequest(int index) {
    if (!sceneChangeQueued) {
        nextSceneIndex = index;
        sceneChangeQueued = true;
    }
}

}