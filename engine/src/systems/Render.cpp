#include "SimpleEngine2D/systems/Render.hpp"

namespace simpleengine2d::systems {

/**
 * Public functions
 */

Render::Render(SDL_Window *window, int index, Uint32 flags) {
    renderer = SDL_CreateRenderer(window, -1, 0);
}

void Render::init() {
    for (auto entity : em.getAllEntities()) {
        if (em.hasComponent<components::MeshComponent>(entity)) {
            components::MeshComponent *mesh = em.getComponent<components::MeshComponent>(entity);

            if (mesh->imagePath != nullptr) {
                SDL_Surface *surface = SDL_LoadBMP(mesh->imagePath);
                SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);
                mesh->texture = texture;
            }
        }
    }
}

void Render::update(float dt) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    core::EntityId camera = em.getAllEntities()[0];
    components::Camera *camComp = em.getComponent<components::Camera>(camera);
    for (auto entity : em.getAllEntities()) {
        if (em.hasComponent<components::TransformComponent>(entity) && em.hasComponent<components::MeshComponent>(entity)) {
            components::TransformComponent *transform = em.getComponent<components::TransformComponent>(entity);
            components::MeshComponent *mesh = em.getComponent<components::MeshComponent>(entity);

            if (camComp->follow == entity) {
                if (camComp->shouldLerp) {
                    glm::vec2 targetPos;
                    targetPos.x = transform->position.x + (transform->scale.x/2) - (1920/2) + camComp->followOffset.x;
                    targetPos.y = transform->position.y + (transform->scale.y/2) - (1088/2) + camComp->followOffset.y;

                    camComp->position += (targetPos-camComp->position) * camComp->smoothingFactor * dt;
                } else {
                    camComp->position.x = transform->position.x + (transform->scale.x/2) - (1920/2) + camComp->followOffset.x;
                    camComp->position.y = transform->position.y + (transform->scale.y/2) - (1088/2) + camComp->followOffset.y;
                }
            }

            mesh->rect = {(int)(transform->position.x - camComp->position.x), (int)(transform->position.y - camComp->position.y), (int)transform->scale.x, (int)transform->scale.y};
            if (mesh->texture != nullptr) {
                SDL_Point center = {mesh->rect.w/2, mesh->rect.h/2};
                SDL_RenderCopyEx(renderer, mesh->texture, nullptr, &mesh->rect, transform->rotation, &center, SDL_FLIP_NONE);
            } else {
                SDL_SetRenderDrawColor(renderer, mesh->color.r, mesh->color.g, mesh->color.b, mesh->color.a);
                SDL_RenderFillRect(renderer, &mesh->rect);
            }
        }

        if (em.hasComponent<components::TransformComponent>(entity) && em.hasComponent<components::ColliderComponent>(entity)) {
            components::TransformComponent *transform = em.getComponent<components::TransformComponent>(entity);
            components::ColliderComponent *collider = em.getComponent<components::ColliderComponent>(entity);

            if (collider->showOutline) {
                collider->rect = {(int)(transform->position.x - camComp->position.x), (int)(transform->position.y - camComp->position.y), (int)transform->scale.x, (int)transform->scale.y};
                SDL_SetRenderDrawColor(renderer, collider->color.r, collider->color.g, collider->color.b, collider->color.a);
                SDL_RenderDrawRect(renderer, &collider->rect);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void Render::clean() {
    SDL_DestroyRenderer(renderer);
}

}