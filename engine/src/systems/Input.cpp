#include "SimpleEngine2D/systems/Input.hpp"

namespace simpleengine2d::systems {

void Input::update(float dt) {
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT) {
            running = false;
        }

        if (event.type == SDL_KEYDOWN) {
            events::InputBegan evt{event.key.keysym.sym};
            core::EventBus::getInstance().publish<events::InputBegan>(&evt);
        }

        if (event.type == SDL_KEYUP) {
            events::InputEnded evt{event.key.keysym.sym};
            eb.publish<events::InputEnded>(&evt);
        }

        if (event.type == SDL_MOUSEMOTION) {
            int x, y;
            SDL_GetMouseState(&x, &y);
            events::MouseMotion evt;
            evt.position = {x, y};
            core::EventBus::getInstance().publish<events::MouseMotion>(&evt);
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            events::MouseButtonDown evt;
            evt.button = event.button.button;
            evt.position.x = event.button.x;
            evt.position.y = event.button.y;
            eb.publish<events::MouseButtonDown>(&evt);
        }

        if (event.type == SDL_MOUSEBUTTONUP) {
            events::MouseButtonUp evt;
            evt.button = event.button.button;
            evt.position.x = event.button.x;
            evt.position.y = event.button.y;
            eb.publish<events::MouseButtonUp>(&evt);
        }
    }
}

}