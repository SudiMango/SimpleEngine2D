#pragma once

#include <SDL2/SDL.h>
#include <map>
#include "SimpleEngine2D/core/System.hpp"
#include "SimpleEngine2D/core/EventBus.hpp"
#include "SimpleEngine2D/events/InputBegan.hpp"
#include "SimpleEngine2D/events/InputEnded.hpp"
#include "SimpleEngine2D/events/MouseMotion.hpp"
#include "SimpleEngine2D/events/MouseButtonDown.hpp"
#include "SimpleEngine2D/events/MouseButtonUp.hpp"

namespace simpleengine2d::systems {

class Input : public core::System {

public:
    Input(bool &running) : running(running) {};

    void init() override {};
    void update(float dt) override;
    void clean() override {};

private:
    SDL_Event event;

    core::EventBus &eb = core::EventBus::getInstance();
    bool &running;

};

}