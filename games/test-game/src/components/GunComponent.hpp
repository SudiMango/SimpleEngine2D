#pragma once

namespace test_game::components {

struct GunComponent {
    float damage = 0.0f;
    float timeBetweenFire = 0.0f;

    float timeSinceLastFire = 0.0f;
};

}