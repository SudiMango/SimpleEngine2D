#pragma once

#include <SDL2/SDL.h>

namespace simpleengine2d::events {

struct InputBegan {
    SDL_Keycode keycode;
};

}