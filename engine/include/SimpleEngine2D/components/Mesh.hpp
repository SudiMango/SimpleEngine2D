#pragma once

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

namespace simpleengine2d::components {

struct MeshComponent {
    SDL_Rect rect = {0, 0, 1, 1};
    glm::ivec4 color = glm::ivec4(255, 0, 0, 255);
    
    char *imagePath = nullptr;
    SDL_Texture *texture;

    bool flipX = false;
    bool flipY = false;
};

}