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

        if (em.hasComponent<components::TextGuiComponent>(entity)) {
            components::TextGuiComponent *textGui = em.getComponent<components::TextGuiComponent>(entity);

            if (textGui->text != nullptr && textGui->fontPath != nullptr) {
                TTF_Font *font = TTF_OpenFont(textGui->fontPath, textGui->textSize);
                SDL_Surface* surface = TTF_RenderText_Solid(font, textGui->text, textGui->textColor);
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);
                TTF_CloseFont(font);
                textGui->texture = texture;
            }
        }
    }
}

std::vector<core::EntityId> Render::buildRenderList() {
    std::vector<core::EntityId> entities = em.getAllEntities();
    auto renderBegin = std::stable_partition(
        entities.begin(), entities.end(),
        [&](core::EntityId e)
        {
            bool hasMeshComponent = em.hasComponent<components::MeshComponent>(e);
            bool hasTextGuiComponent = em.hasComponent<components::TextGuiComponent>(e);
            bool hasPanelGuiComponent = em.hasComponent<components::PanelGuiComponent>(e);

            return !(hasMeshComponent || hasTextGuiComponent || hasPanelGuiComponent);
        }
    );

    auto getZIndex = [&](core::EntityId e) {
        if (em.hasComponent<components::MeshComponent>(e)) {
            return em.getComponent<components::MeshComponent>(e)->zIndex;
        } else if (em.hasComponent<components::TextGuiComponent>(e)) {
            return em.getComponent<components::TextGuiComponent>(e)->zIndex;
        } else if (em.hasComponent<components::PanelGuiComponent>(e)) {
            return em.getComponent<components::PanelGuiComponent>(e)->zIndex;
        }

        return 0;
    };

    std::sort(
        renderBegin, entities.end(),
        [&](core::EntityId a, core::EntityId b)
        {
            return getZIndex(a) < getZIndex(b);
        }
    );

    return entities;
}

void Render::update(float dt) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    
    for (auto entity : em.getAllEntities()) {
        // Adjust all welded components
        if (em.hasComponent<components::TransformComponent>(entity) && em.hasComponent<components::WeldComponent>(entity)) {
            components::TransformComponent *transform = em.getComponent<components::TransformComponent>(entity);
            components::WeldComponent *weld = em.getComponent<components::WeldComponent>(entity);
            for (auto w : weld->welds) {
                if (em.hasComponent<components::TransformComponent>(w.child)) {
                    components::TransformComponent *child_t = em.getComponent<components::TransformComponent>(w.child);
                    child_t->position = transform->position + w.positionOffset;

                    if (w.followRotation) {
                        child_t->rotation = transform->rotation;
                    }
                }
            }
        }

        // Ensure all newly added entities have their surfaces loaded
        if (em.hasComponent<components::MeshComponent>(entity)) {
            components::MeshComponent *mesh = em.getComponent<components::MeshComponent>(entity);

            if (mesh->imagePath != nullptr && mesh->texture == nullptr) {
                SDL_Surface *surface = SDL_LoadBMP(mesh->imagePath);
                SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_FreeSurface(surface);
                mesh->texture = texture;
            }
        }
    }

    // Render all entities
    core::EntityId camera = em.getAllEntities()[0];
    components::Camera *camComp = em.getComponent<components::Camera>(camera);

    std::vector<core::EntityId> entities = buildRenderList();
    for (auto entity : entities) {

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
                SDL_RendererFlip flip = SDL_FLIP_NONE;
                if (mesh->flipX && mesh->flipY) {
                    flip = (SDL_RendererFlip)(SDL_FLIP_HORIZONTAL | SDL_FLIP_VERTICAL);
                } else if (mesh->flipX && !mesh->flipY) {
                    flip = SDL_FLIP_HORIZONTAL;
                } else if (!mesh->flipX && mesh->flipY) {
                    flip = SDL_FLIP_VERTICAL;
                }
                SDL_RenderCopyEx(renderer, mesh->texture, nullptr, &mesh->rect, transform->rotation, &transform->anchor, flip);
            } else {
                SDL_SetRenderDrawColor(renderer, mesh->color.r, mesh->color.g, mesh->color.b, mesh->color.a);
                SDL_RenderFillRect(renderer, &mesh->rect);
            }
        }

        // Render colliders
        if (em.hasComponent<components::TransformComponent>(entity) && em.hasComponent<components::ColliderComponent>(entity)) {
            components::TransformComponent *transform = em.getComponent<components::TransformComponent>(entity);
            components::ColliderComponent *collider = em.getComponent<components::ColliderComponent>(entity);

            if (collider->showOutline) {
                float width = (collider->scale.x > 0) ? collider->scale.x : transform->scale.x;
                float height = (collider->scale.y > 0) ? collider->scale.y : transform->scale.y;

                glm::vec2 localCenter = { (transform->scale.x / 2.0f), (transform->scale.y / 2.0f) };

                float dx = localCenter.x - transform->anchor.x;
                float dy = localCenter.y - transform->anchor.y;

                float angle = glm::radians(transform->rotation);
                float cosA = cos(angle);
                float sinA = sin(angle);
                
                float rx = dx * cosA - dy * sinA;
                float ry = dx * sinA + dy * cosA;

                glm::vec2 worldCenter = {
                    transform->position.x + transform->anchor.x + rx,
                    transform->position.y + transform->anchor.y + ry
                };

                collider->rect.w = (int)width;
                collider->rect.h = (int)height;
                collider->rect.x = (int)(worldCenter.x - (width / 2.0f) - camComp->position.x);
                collider->rect.y = (int)(worldCenter.y - (height / 2.0f) - camComp->position.y);
                
                SDL_SetRenderDrawColor(renderer, collider->color.r, collider->color.g, collider->color.b, collider->color.a);
                SDL_RenderDrawRect(renderer, &collider->rect);
            }
        }

        // Render panels
        if (em.hasComponent<components::PanelGuiComponent>(entity)) {
            auto *panelGui = em.getComponent<components::PanelGuiComponent>(entity);

            int x = (int)panelGui->position.x;
            int y = (int)panelGui->position.y;
            if (panelGui->inWorldSpace) {
                x -= (int)camComp->position.x;
                y -= (int)camComp->position.y;
            }

            panelGui->rect = {x, y, (int)panelGui->scale.x, (int)panelGui->scale.y};
            SDL_SetRenderDrawColor(renderer, panelGui->color.r, panelGui->color.g, panelGui->color.b, panelGui->color.a);
            SDL_RenderFillRect(renderer, &panelGui->rect);
        }

        // Render text
        if (em.hasComponent<components::TextGuiComponent>(entity)) {
            auto *textGui = em.getComponent<components::TextGuiComponent>(entity);

            int textW = 0, textH = 0;
            SDL_QueryTexture(textGui->texture, nullptr, nullptr, &textW, &textH);

            int x = (int)textGui->position.x;
            int y = (int)textGui->position.y;
            if (textGui->inWorldSpace) {
                x -= (int)camComp->position.x;
                y -= (int)camComp->position.y;
            }

            SDL_Rect dst = { x, y, textW, textH };
            SDL_RenderCopy(renderer, textGui->texture, nullptr, &dst);
        }
    }

    SDL_RenderPresent(renderer);
}

void Render::clean() {
    SDL_DestroyRenderer(renderer);
}

}