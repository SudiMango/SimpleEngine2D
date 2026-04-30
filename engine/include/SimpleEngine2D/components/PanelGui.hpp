#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "SimpleEngine2D/util/glm_json.hpp"
#include "SimpleEngine2D/util/sdl_json.hpp"

namespace simpleengine2d::components {

struct PanelGuiComponent {
    glm::ivec4 color = {255, 255, 255, 255};
    bool inWorldSpace = false;
    int zIndex = 100;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PanelGuiComponent, color, inWorldSpace, zIndex)

}