#pragma once

#include <SDL2/SDL.h>
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <SDL_mixer.h>
#include "SimpleEngine2D/core/System.hpp"
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/core/SceneManager.hpp"
#include "SimpleEngine2D/core/ConfigManager.hpp"
#include "SimpleEngine2D/core/TagManager.hpp"

#include "SimpleEngine2D/systems/Input.hpp"
#include "SimpleEngine2D/systems/Render.hpp"
#include "SimpleEngine2D/systems/Collision.hpp"
#include "SimpleEngine2D/systems/Physics.hpp"
#include "SimpleEngine2D/systems/Audio.hpp"

#include "SimpleEngine2D/events/RequestSceneChange.hpp"

#include "SimpleEngine2D/components/Camera.hpp"

namespace simpleengine2d::core {

class Engine {

public:
    static Engine &getInstance() {
        static Engine instance;
        return instance;
    }

    void addSystem(core::System *system) {
        systems.push_back(system);
    }
    
    components::Camera *getCamera() {
        return em.getComponent<components::Camera>((EntityId)0);
    }
    
    void init(int sceneIndex);
    void run();

private:

    /**
     * Singleton stuff
     */
    Engine() = default;
    ~Engine() = default;

    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;

    /**
     * Core variables
     */
    bool running = false;
    std::vector<System*> systems;
    SDL_Window *window;
    components::Camera *camera;

    core::EntityManager &em = core::EntityManager::getInstance();
    core::Config &config = core::ConfigManager::getInstance().getConfig();

    /**
     * Time related stuff
     */
    float dt = 0.0f;
    float fixedUpdateAcc = 0.0f;
    std::chrono::steady_clock::time_point lastFrameTime;
    const float FIXED_UPDATE_DELAY = 0.02f;

    /**
     * Scene stuff
     */
    bool sceneChangeQueued = false;
    int nextSceneIndex = -1;

    /**
     * Private functions
     */
    void onChangeSceneRequest(int index);
    void setup();
    void clean();
};

}
