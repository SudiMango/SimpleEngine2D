#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "events/EnemyDrops.hpp"

namespace test_game::events {

struct OnEnemyDeath {
    glm::vec2 pos;
    std::vector<events::EnemyDrops> drops;
};

}