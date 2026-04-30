#pragma once

#include <string>
#include "SimpleEngine2D/util/glm_json.hpp"
#include "SimpleEngine2D/util/sdl_json.hpp"

namespace simpleengine2d::components {

struct AudioComponent {
    bool isMusic = false;
    std::string filePath = "";
    int numLoops = 0; // 0 = no loops, -1 = infinite loops
    int volume = 128; // 0 - 128
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(AudioComponent, isMusic, filePath, numLoops, volume)

}