#include "systems/EnemySystem.hpp"

namespace test_game::systems {

/**
 * Public functions
 */

EnemySystem::EnemySystem(core::EntityId player, const std::vector<core::EntityId>& enemyIds)
    : player(player), enemies(enemyIds) {
    for (const auto& enemyId : enemies) {
        enemyStates[enemyId] = EnemyState{};
    }
}

void EnemySystem::init() {
    core::EventBus::getInstance().subscribe<simpleengine2d::events::CollisionEnter>(this, [this](void *evt) {
        auto *_evt = (simpleengine2d::events::CollisionEnter*)evt;

        auto it1 = bullets.find(_evt->entity1);
        auto it2 = bullets.find(_evt->entity2);

        if (it1 != bullets.end() && it2 != bullets.end()) {

        } else if (it1 != bullets.end()) {
            if (em.hasComponent<components::HealthComponent>(_evt->entity2)) {
                auto *dmg = em.getComponent<components::DamageComponent>(_evt->entity1);
                auto *h = em.getComponent<components::HealthComponent>(_evt->entity2);

                h->currHealth -= dmg->damage;
            }
            em.deleteEntity(_evt->entity1);
            bullets.erase(_evt->entity1);
        } else if (it2 != bullets.end()) {
            if (em.hasComponent<components::HealthComponent>(_evt->entity1)) {
                auto *dmg = em.getComponent<components::DamageComponent>(_evt->entity2);
                auto *h = em.getComponent<components::HealthComponent>(_evt->entity1);

                h->currHealth -= dmg->damage;
            }

            em.deleteEntity(_evt->entity2);
            bullets.erase(_evt->entity2);
        }
    });

    for (const auto& enemyId : enemies) {
        auto *ec = em.getComponent<components::EnemyComponent>(enemyId);
        auto *t = em.getComponent<simpleengine2d::components::TransformComponent>(enemyId);
        ec->lastPosX = t->position.x;
        initUi(enemyId);
    }

    core::EventBus::getInstance().subscribe<events::OnEnemyDeath>(this, [this](void *evt) {
        auto *_evt = (events::OnEnemyDeath*)evt;

        for(auto item : _evt->drops) {
            auto drop = em.createEntity();
            auto *t = new simpleengine2d::components::TransformComponent();
            t->position = {_evt->pos.x + simpleengine2d::util::getRandomFloat(-50.0f, 50.0f), _evt->pos.y};
            t->scale = {30, 30};
            auto *m = new simpleengine2d::components::MeshComponent();
            auto *rb = new simpleengine2d::components::RigidBodyComponent();
            rb->useGravity = true;
            rb->velocity.y -= 750.0f;
            auto *c = new simpleengine2d::components::ColliderComponent();
            c->encompassTransform = true;

            if (item == events::BANDAGE) {
                m->imagePath = (char*)"./games/test-game/assets/textures/bandage.bmp";
                auto *health = new components::DamageComponent();
                health->damage = simpleengine2d::util::getRandomFloat(10.0f, 50.0f);
                em.addComponent<components::DamageComponent>(drop, health);
            } else if (item == events::AMMO) {
                m->imagePath = (char*)"./games/test-game/assets/textures/bulleticon.bmp";
                auto *ammo = new components::AmmoComponent();
                ammo->ammo = simpleengine2d::util::getRandomInt(5, 15);
                em.addComponent<components::AmmoComponent>(drop, ammo);
            }

            em.addComponent<simpleengine2d::components::TransformComponent>(drop, t);
            em.addComponent<simpleengine2d::components::RigidBodyComponent>(drop, rb);
            em.addComponent<simpleengine2d::components::ColliderComponent>(drop, c);
            em.addComponent<simpleengine2d::components::MeshComponent>(drop, m);
            enemyDrops.push_back(drop);
        }
    });

    core::EventBus::getInstance().subscribe<simpleengine2d::events::CollisionEnter>(this, [this](void *evt) {
        auto *_evt = (simpleengine2d::events::CollisionEnter*)evt;

        auto it1 = std::find(enemyDrops.begin(), enemyDrops.end(), _evt->entity1);
        auto it2 = std::find(enemyDrops.begin(), enemyDrops.end(), _evt->entity2);
        if (it1 != enemyDrops.end()) {
            if (_evt->entity2 == player) {
                if (em.hasComponent<components::DamageComponent>(_evt->entity1)) {
                    auto *dmg = em.getComponent<components::DamageComponent>(_evt->entity1);
                    auto *p_h = em.getComponent<components::HealthComponent>(_evt->entity2);

                    p_h->currHealth += dmg->damage;
                } else if (em.hasComponent<components::AmmoComponent>(_evt->entity1)) {
                    events::AddAmmo ammoEvt;
                    ammoEvt.ammo = em.getComponent<components::AmmoComponent>(_evt->entity1)->ammo;
                    core::EventBus::getInstance().publish<events::AddAmmo>(&ammoEvt);
                }

                em.deleteEntity(_evt->entity1);
                enemyDrops.erase(it1);
            }
        } else if (it2 != enemyDrops.end()) {
            if (_evt->entity1 == player) {
                if (em.hasComponent<components::DamageComponent>(_evt->entity2)) {
                    auto *dmg = em.getComponent<components::DamageComponent>(_evt->entity2);
                    auto *p_h = em.getComponent<components::HealthComponent>(_evt->entity1);

                    p_h->currHealth += dmg->damage;
                } else if (em.hasComponent<components::AmmoComponent>(_evt->entity2)) {
                    events::AddAmmo ammoEvt;
                    ammoEvt.ammo = em.getComponent<components::AmmoComponent>(_evt->entity2)->ammo;
                    core::EventBus::getInstance().publish<events::AddAmmo>(&ammoEvt);
                }

                em.deleteEntity(_evt->entity2);
                enemyDrops.erase(it2);
            }
        }
    });
}

void EnemySystem::update(float dt) {
    // Update bullets
    for (auto it = bullets.begin(); it != bullets.end(); ) {
        it->second += dt;
        if (it->second >= MAX_ALIVE_TIME) {
            em.deleteEntity(it->first);
            it = bullets.erase(it);
        } else {
            ++it;
        }
    }

    // Remove dead enemies
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [this](core::EntityId enemyId) {
            auto *h = em.getComponent<components::HealthComponent>(enemyId);
            if ((h && h->currHealth <= 0)) {
                events::OnEnemyDeath evt;
                auto *t = em.getComponent<simpleengine2d::components::TransformComponent>(enemyId);
                evt.pos = {t->position.x + t->scale.x/2, t->position.y + t->scale.y/2};
                evt.drops.push_back(events::BANDAGE);
                evt.drops.push_back(events::AMMO);
                core::EventBus::getInstance().publish<events::OnEnemyDeath>(&evt);
                enemyStates.erase(enemyId);
                em.deleteEntity(enemyId);
                return true;
            }
            return false;
        }), enemies.end());


    // Update live enemies
    for (const auto& enemyId : enemies) {
        auto *ec = em.getComponent<components::EnemyComponent>(enemyId);
        auto *rb = em.getComponent<simpleengine2d::components::RigidBodyComponent>(enemyId);
        auto *t = em.getComponent<simpleengine2d::components::TransformComponent>(enemyId);
        auto *m = em.getComponent<simpleengine2d::components::MeshComponent>(enemyId);
        auto *enemyWeld = em.getComponent<simpleengine2d::components::WeldComponent>(enemyId);
        auto *hComp = em.getComponent<components::HealthComponent>(enemyId);

        auto pistol = enemyWeld->welds[0].child;
        auto *gunWeld = em.getComponent<simpleengine2d::components::WeldComponent>(pistol);
        auto *t_gun = em.getComponent<simpleengine2d::components::TransformComponent>(pistol);
        auto *gun = em.getComponent<components::GunComponent>(pistol);
        gun->timeSinceLastFire += dt;

        auto *t_player = em.getComponent<simpleengine2d::components::TransformComponent>(player);

        glm::vec2 g_center = {t_gun->position.x + (t_gun->scale.x / 2), t_gun->position.y + (t_gun->scale.y / 2)};
        glm::vec2 p_center = {t_player->position.x + (t_player->scale.x / 2), t_player->position.y + (t_player->scale.y / 2)};
        glm::vec2 dir = p_center - g_center;

        ec->distanceCovered = std::abs(t->position.x - ec->lastPosX);

        // Switch states
        if (glm::length(dir) <= ec->playerDetectionRange && ec->state == components::EnemyComponent::PATROLLING) {
            ec->state = components::EnemyComponent::SHOOTING;
        } else if (glm::length(dir) > ec->playerDetectionRange && ec->state == components::EnemyComponent::SHOOTING) {
            ec->state = components::EnemyComponent::PATROLLING;
        }

        auto& state = enemyStates[enemyId];

        // Flipping enemy
        if (!state.facingRight && !m->flipX) {
            m->flipX = true;
            em.getComponent<simpleengine2d::components::MeshComponent>(pistol)->flipY = true;

            enemyWeld->welds[0].positionOffset.x -= t->scale.x;
            t_gun->rotation = 180;

            for (auto &w : gunWeld->welds) {
                auto *t1 = em.getComponent<simpleengine2d::components::TransformComponent>(w.child);
                t1->anchor.y -= 20;
                w.positionOffset.y += 20;
            }
        } else if (state.facingRight && m->flipX) {
            m->flipX = false;
            em.getComponent<simpleengine2d::components::MeshComponent>(pistol)->flipY = false;

            enemyWeld->welds[0].positionOffset.x += t->scale.x;
            t_gun->rotation = 0;

            for (auto &w : gunWeld->welds) {
                auto *t1 = em.getComponent<simpleengine2d::components::TransformComponent>(w.child);
                t1->anchor.y += 20;
                w.positionOffset.y -= 20;
            }
        }

        // Moving enemy
        if (ec->state == components::EnemyComponent::PATROLLING) {
            rb->velocity.x = rb->maxVelocity * ec->moveDir; 

            if (ec->distanceCovered >= ec->patrolDistance) {
                ec->moveDir *= -1;
                ec->distanceCovered = 0;
                ec->lastPosX = t->position.x;
                state.facingRight = !state.facingRight;
            }
        } else if (ec->state == components::EnemyComponent::SHOOTING) {
            rb->velocity.x = 0; 

            if ((t_player->position.x + (t_player->scale.x/2)) >= (t->position.x + (t->scale.x/2)) && !state.facingRight) {
                state.facingRight = true;
            } else if ((t_player->position.x + (t_player->scale.x/2)) < (t->position.x + (t->scale.x/2)) && state.facingRight) {
                state.facingRight = false;
            }
        }

        // Rotating gun
        if (ec->state == components::EnemyComponent::SHOOTING) {
            float angle = glm::degrees(glm::atan(dir.y, dir.x));
            if (state.facingRight) {
                angle = std::clamp(angle, -90.0f, 90.0f);
            } else {
                // normalize to -180, 180
                if (angle >= 180.0f) {
                    angle -= 360.0f;
                }
                if (angle <  -180.0f) {
                    angle += 360.0f;
                }

                // clamp to [-180,-90] U [90,180]
                if (angle > -90.0f && angle < 90.0f) {
                    angle = (angle < 0.0f) ? -90.0f : 90.0f;
                }
            }
            t_gun->rotation = angle;
        } else {
            if (state.facingRight && t_gun->rotation != 0) {
                t_gun->rotation = 0;
            } else if (!state.facingRight && t_gun->rotation != 180) {
                t_gun->rotation = 180;
            }
        }

        // Shooting gun
        if (ec->state == components::EnemyComponent::SHOOTING) {
            if (gun->timeSinceLastFire >= gun->timeBetweenFire) {
                createBullet(em.getComponent<simpleengine2d::components::TransformComponent>(gunWeld->welds[0].child), pistol, enemyId);
                gun->timeSinceLastFire = 0;
            }
        }

        // Health UI
        if(state.healthBar) {
            state.healthBar->scale.x = std::clamp((hComp->currHealth/hComp->maxHealth) * healthBarMaxWidth, 0.0f, healthBarMaxWidth);
        }
    }
}

/**
 * Private functions
 */

void EnemySystem::createBullet(simpleengine2d::components::TransformComponent *bulletSpawn, core::EntityId gun, core::EntityId owner) {
    auto *gunComp = em.getComponent<components::GunComponent>(gun);

    auto bullet = em.createEntity();
    auto *bulletTransform = new simpleengine2d::components::TransformComponent();
    bulletTransform->rotation = bulletSpawn->rotation;
    bulletTransform->scale = {25, 4};
    bulletTransform->position = {bulletSpawn->position.x + (bulletSpawn->scale.x/2) - (bulletTransform->scale.x/2), bulletSpawn->position.y + (bulletSpawn->scale.y/2) - (bulletTransform->scale.y/2)};
    bulletTransform->anchor = {(int)(bulletSpawn->anchor.x - (bulletSpawn->scale.x/2) + (bulletTransform->scale.x/2)), (int)(bulletSpawn->anchor.y - (bulletSpawn->scale.y/2) + (bulletTransform->scale.y/2))};
    auto *mesh = new simpleengine2d::components::MeshComponent();
    mesh->imagePath = (char*)"./games/test-game/assets/textures/bullet.bmp";
    mesh->zIndex = 5;
    auto *rb = new simpleengine2d::components::RigidBodyComponent();
    rb->maxVelocity = 1000;
    glm::vec2 dir = {glm::cos(glm::radians(bulletTransform->rotation)), glm::sin(glm::radians(bulletTransform->rotation))};
    rb->velocity = rb->maxVelocity * dir;
    auto *collider = new simpleengine2d::components::ColliderComponent();
    collider->isTrigger = true;
    collider->encompassTransform = true;
    collider->ignore.push_back(owner);
    collider->ignore.push_back(gun);
    auto *dmg = new components::DamageComponent();
    dmg->damage = gunComp->damage;
    em.addComponent<simpleengine2d::components::TransformComponent>(bullet, bulletTransform);
    em.addComponent<simpleengine2d::components::MeshComponent>(bullet, mesh);
    em.addComponent<simpleengine2d::components::RigidBodyComponent>(bullet, rb);
    em.addComponent<simpleengine2d::components::ColliderComponent>(bullet, collider);
    em.addComponent<components::DamageComponent>(bullet, dmg);

    bullets[bullet] = 0.0f;
}

void EnemySystem::initUi(core::EntityId enemy) {
    auto bg = em.createEntity();
    auto *t0 = new simpleengine2d::components::TransformComponent();
    t0->scale = {healthBarMaxWidth, 20};
    auto *bgPanel = new simpleengine2d::components::PanelGuiComponent();
    bgPanel->color = {189, 189, 189, 255};
    bgPanel->inWorldSpace = true;
    bgPanel->zIndex = 98;
    em.addComponent<simpleengine2d::components::TransformComponent>(bg, t0);
    em.addComponent<simpleengine2d::components::PanelGuiComponent>(bg, bgPanel);

    auto e = em.createEntity();
    auto *t1 = new simpleengine2d::components::TransformComponent();
    t1->scale = {healthBarMaxWidth, 20};
    auto *healthPanel = new simpleengine2d::components::PanelGuiComponent();
    healthPanel->color = {0, 255, 0, 255};
    healthPanel->inWorldSpace = true;
    healthPanel->zIndex = 99;
    em.addComponent<simpleengine2d::components::TransformComponent>(e, t1);
    em.addComponent<simpleengine2d::components::PanelGuiComponent>(e, healthPanel);
    enemyStates[enemy].healthBar = t1;

    auto *weld = new simpleengine2d::components::WeldComponent();
    simpleengine2d::components::_welddata healthToBg;
    healthToBg.child = e;
    weld->welds.push_back(healthToBg);
    em.addComponent<simpleengine2d::components::WeldComponent>(bg, weld);

    auto *weld2 = em.getComponent<simpleengine2d::components::WeldComponent>(enemy);
    simpleengine2d::components::_welddata uiToEnemy;
    uiToEnemy.child = bg;
    uiToEnemy.positionOffset = {0, -30};
    weld2->welds.push_back(uiToEnemy);
}

}