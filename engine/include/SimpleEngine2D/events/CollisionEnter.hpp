#pragma once

#include "SimpleEngine2D/core/EntityManager.hpp"

namespace simpleengine2d::events {

enum CollisionAxis {
    X,
    Y
};

struct CollisionEnter
{
    core::EntityId entity1;
    core::EntityId entity2;
    CollisionAxis axis;
};

}
