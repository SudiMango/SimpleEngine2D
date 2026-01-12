#pragma once

#include <vector>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>

#include "SimpleEngine2D/util/Utils.hpp"

#include "SimpleEngine2D/core/System.hpp"
#include "SimpleEngine2D/core/EventBus.hpp"
#include "SimpleEngine2D/core/EntityManager.hpp"

#include "SimpleEngine2D/events/CollisionEnter.hpp"

#include "events/AddAmmo.hpp"
#include "events/EnemyDrops.hpp"
#include "events/OnEnemyDeath.hpp"

#include "SimpleEngine2D/components/Transform.hpp"
#include "SimpleEngine2D/components/Mesh.hpp"
#include "SimpleEngine2D/components/Weld.hpp"
#include "SimpleEngine2D/components/RigidBody.hpp"
#include "SimpleEngine2D/components/PanelGui.hpp"
#include "SimpleEngine2D/components/Collider.hpp"

#include "components/EnemyComponent.hpp"
#include "components/GunComponent.hpp"
#include "components/HealthComponent.hpp"
#include "components/DamageComponent.hpp"
#include "components/AmmoComponent.hpp"

using namespace simpleengine2d;

namespace test_game::systems {

class EnemySystem : public core::System {

public:
    EnemySystem(core::EntityId player, const std::vector<core::EntityId>& enemyIds);

    void init() override;
    void update(float dt) override;
    void clean() override {};

private:
    struct EnemyState {
        bool facingRight = true;
        simpleengine2d::components::TransformComponent* healthBar = nullptr;
    };

    core::EntityManager &em = core::EntityManager::getInstance();
    core::EntityId player;
    std::vector<core::EntityId> enemies;
    std::map<core::EntityId, EnemyState> enemyStates;
    std::vector<core::EntityId> enemyDrops;

    std::map<core::EntityId, float> bullets;
    const float MAX_ALIVE_TIME = 5.0f;

    float healthBarMaxWidth = 100.0f;

    void createBullet(simpleengine2d::components::TransformComponent *bulletSpawn, core::EntityId gun, core::EntityId owner);
    void initUi(core::EntityId enemy);

};

}