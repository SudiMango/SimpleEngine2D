#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <string>
#include "SimpleEngine2D/util/glm_json.hpp"
#include "SimpleEngine2D/util/sdl_json.hpp"

namespace simpleengine2d::components {

struct TextGuiComponent {
    std::string text;
    std::string fontPath = "";
    int textSize = 24;
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Texture *texture = nullptr;
    bool inWorldSpace = false;
    int zIndex = 100;
    glm::ivec2 textureScale = glm::ivec2(0);
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TextGuiComponent, text, fontPath, textSize, textColor, inWorldSpace, zIndex, textureScale)

}
