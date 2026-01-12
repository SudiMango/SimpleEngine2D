#pragma once

namespace test_game::components {

struct EnemyComponent {
    enum EnemyState {
        PATROLLING,
        SHOOTING
    };

    float patrolDistance = 0.0f;
    float distanceCovered = 0.0f;
    float lastPosX = 0.0f;
    float moveDir = 1;
    EnemyState state = PATROLLING;
    float playerDetectionRange = 0.0f;
};

}