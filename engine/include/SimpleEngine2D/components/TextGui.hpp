#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <string>

namespace simpleengine2d::components {

struct TextGuiComponent {
    std::string text;
    char *fontPath = nullptr;
    int textSize = 24;
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Texture *texture;
    bool inWorldSpace = false;
    int zIndex = 100;
    glm::ivec2 textureScale = glm::ivec2(0);
};

}