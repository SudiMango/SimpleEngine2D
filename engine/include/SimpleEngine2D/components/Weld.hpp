#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/util/glm_json.hpp"
#include "SimpleEngine2D/util/sdl_json.hpp"

namespace simpleengine2d::components {

struct _welddata {
    core::EntityId child;
    glm::vec2 positionOffset = glm::vec2(0.0f);
    bool followRotation = false;
};

struct WeldComponent {
    std::vector<_welddata> welds;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(_welddata, child, positionOffset, followRotation)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(WeldComponent, welds)

}