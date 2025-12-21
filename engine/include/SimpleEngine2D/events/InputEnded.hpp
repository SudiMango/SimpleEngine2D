#pragma once

#include <SDL2/SDL.h>

namespace simpleengine2d::events {

struct InputEnded {
    SDL_Keycode keycode;
};

}