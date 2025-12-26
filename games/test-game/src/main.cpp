#include "SimpleEngine2D/core/Engine.hpp"
#include "SimpleEngine2D/components/Transform.hpp"
#include "SimpleEngine2D/components/Mesh.hpp"
#include "SimpleEngine2D/components/RigidBody.hpp"
#include "SimpleEngine2D/components/Collider.hpp"
#include "systems/MovementSystem.hpp"

int main(int argc, char *argv[]) {

    auto *engine = new simpleengine2d::core::Engine();
    engine->init("Engine", 1920, 1088, false);

    // add logic here
    auto &em = engine->getEntityManager();

    simpleengine2d::core::EntityId player = em.createEntity();
    auto *transform = new simpleengine2d::components::TransformComponent();
    transform->position = {(1920/2) - 100, (1088/2) - 100};
    transform->scale = {200, 200};
    auto *mesh = new simpleengine2d::components::MeshComponent();
    mesh->imagePath = (char *)"./games/test-game/assets/textures/raeed.bmp";
    auto *rb = new simpleengine2d::components::RigidBodyComponent();
    rb->maxVelocity = 500;
    rb->useGravity = false;
    auto *collider = new simpleengine2d::components::ColliderComponent();
    em.addComponent<simpleengine2d::components::TransformComponent>(player, transform);
    em.addComponent<simpleengine2d::components::MeshComponent>(player, mesh);
    em.addComponent<simpleengine2d::components::RigidBodyComponent>(player, rb);
    em.addComponent<simpleengine2d::components::ColliderComponent>(player, collider);

    simpleengine2d::core::EntityId block = em.createEntity();
    auto *transform2 = new simpleengine2d::components::TransformComponent();
    transform2->position = {500, 500};
    transform2->scale = {200, 200};
    auto *mesh2 = new simpleengine2d::components::MeshComponent();
    mesh2->imagePath = (char *)"./games/test-game/assets/textures/sudi.bmp";
    auto *collider2 = new simpleengine2d::components::ColliderComponent();
    em.addComponent<simpleengine2d::components::TransformComponent>(block, transform2);
    em.addComponent<simpleengine2d::components::MeshComponent>(block, mesh2);
    em.addComponent<simpleengine2d::components::ColliderComponent>(block, collider2);

    simpleengine2d::core::EntityId floor = em.createEntity();
    auto *transform3 = new simpleengine2d::components::TransformComponent();
    transform3->position = {500, 800};
    transform3->scale = {1000, 200};
    auto *mesh3 = new simpleengine2d::components::MeshComponent();
    mesh3->imagePath = (char *)"./games/test-game/assets/textures/achiles.bmp";
    auto *collider3 = new simpleengine2d::components::ColliderComponent();
    em.addComponent<simpleengine2d::components::TransformComponent>(floor, transform3);
    em.addComponent<simpleengine2d::components::MeshComponent>(floor, mesh3);
    em.addComponent<simpleengine2d::components::ColliderComponent>(floor, collider3);

    simpleengine2d::core::EntityId wall = em.createEntity();
    auto *wt = new simpleengine2d::components::TransformComponent();
    wt->position = {1700, 0};
    wt->scale = {100, 1000};
    auto *wm = new simpleengine2d::components::MeshComponent();
    wm->imagePath = (char *)"./games/test-game/assets/textures/malir.bmp";
    auto *wc = new simpleengine2d::components::ColliderComponent();
    em.addComponent<simpleengine2d::components::TransformComponent>(wall, wt);
    em.addComponent<simpleengine2d::components::MeshComponent>(wall, wm);
    em.addComponent<simpleengine2d::components::ColliderComponent>(wall, wc);

    engine->addSystem(new test_game::systems::MovementSystem(player));
    
    engine->run();

    return 0;
}
