#pragma once

#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/util/CollisionAxis.hpp"

namespace simpleengine2d::events {

struct CollisionExit
{
    core::EntityId entity1;
    core::EntityId entity2;

    util::CollisionAxis axis;
};

}
