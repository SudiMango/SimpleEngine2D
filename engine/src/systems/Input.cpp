#include "SimpleEngine2D/systems/Input.hpp"

namespace simpleengine2d::systems {

void Input::update(float dt) {
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) {
            running = false;
        }

        if (event.type == SDL_KEYDOWN) {
            if (!keyStatesDown[event.key.keysym.sym]) {
                keyStatesDown[event.key.keysym.sym] = true;
                events::InputBegan evt{event.key.keysym.sym};
                core::EventBus::getInstance().publish<events::InputBegan>(&evt);
            }
        }

        if (event.type == SDL_KEYUP) {
            if (keyStatesDown[event.key.keysym.sym]) {
                keyStatesDown[event.key.keysym.sym] = false;
                events::InputEnded evt{event.key.keysym.sym};
                core::EventBus::getInstance().publish<events::InputEnded>(&evt);
            }
        }
    }
}

}