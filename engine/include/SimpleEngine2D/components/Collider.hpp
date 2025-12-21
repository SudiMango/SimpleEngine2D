#pragma once

#include <SDL2/SDL.h>

namespace simpleengine2d::components {

struct ColliderComponent {
    SDL_Rect collider = {0, 0, 1, 1};
    bool showOutline = false;
};

}