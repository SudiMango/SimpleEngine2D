#pragma once

#include <glm/glm.hpp>

#include "SimpleEngine2D/core/EntityManager.hpp"

#include "SimpleEngine2D/components/Transform.hpp"
#include "SimpleEngine2D/components/Mesh.hpp"
#include "SimpleEngine2D/components/Weld.hpp"
#include "SimpleEngine2D/components/RigidBody.hpp"
#include "SimpleEngine2D/components/Collider.hpp"

#include "components/HealthComponent.hpp"
#include "components/EnemyComponent.hpp"
#include "components/GunComponent.hpp"

namespace test_game::factories {

class EnemyFactory {
public:
    simpleengine2d::core::EntityId create(glm::vec2 position);

private:
    simpleengine2d::core::EntityManager& em = simpleengine2d::core::EntityManager::getInstance();
};

}