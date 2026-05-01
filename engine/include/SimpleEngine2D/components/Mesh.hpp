#pragma once

#include <string>
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include "SimpleEngine2D/util/glm_json.hpp"
#include "SimpleEngine2D/util/sdl_json.hpp"

namespace simpleengine2d::components {

struct MeshComponent {
    SDL_Rect rect = {0, 0, 1, 1};
    glm::ivec4 color = glm::ivec4(255, 0, 0, 255);

    std::string imagePath = "";
    std::string prevImagePath = "";
    SDL_Texture *texture = nullptr;

    bool flipX = false;
    bool flipY = false;

    int zIndex = 0;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MeshComponent, rect, imagePath, color, flipX, flipY, zIndex)

}
