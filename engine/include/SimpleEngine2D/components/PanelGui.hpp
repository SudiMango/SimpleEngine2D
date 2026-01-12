#pragma once

#include <glm/glm.hpp>
#include <SDL2/SDL.h>

namespace simpleengine2d::components {

struct PanelGuiComponent {
    glm::ivec4 color = {255, 255, 255, 255};
    bool inWorldSpace = false;
    int zIndex = 100;
};

}