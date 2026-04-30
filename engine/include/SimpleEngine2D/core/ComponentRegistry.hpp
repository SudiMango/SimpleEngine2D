#pragma once
#include <functional>
#include <unordered_map>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "imgui.h"
#include "SimpleEngine2D/core/EntityManager.hpp"
#include "SimpleEngine2D/components/Audio.hpp"
#include "SimpleEngine2D/components/Camera.hpp"
#include "SimpleEngine2D/components/Collider.hpp"
#include "SimpleEngine2D/components/Mesh.hpp"
#include "SimpleEngine2D/components/PanelGui.hpp"
#include "SimpleEngine2D/components/RigidBody.hpp"
#include "SimpleEngine2D/components/TextGui.hpp"
#include "SimpleEngine2D/components/Transform.hpp"
#include "SimpleEngine2D/components/Weld.hpp"

using json = nlohmann::json;

namespace simpleengine2d::core {

using SerializeFn = std::function<json(EntityId)>;
using DeserializeFn = std::function<void(EntityId, const json&)>;
using CheckerFn = std::function<bool(EntityId)>;
using InspectFn = std::function<void(EntityId)>;
using DefaultFn = std::function<void(EntityId)>;
using RemoveFn = std::function<void(EntityId)>;

class ComponentRegistry {
public:
    static ComponentRegistry& getInstance() {
        static ComponentRegistry instance;
        return instance;
    }

    template<typename T>
    void registerComponent(const std::string& name, std::function<void(EntityId, T&)> inspect = nullptr) {
        serializers[name] = [this](EntityId id) -> json {
            return *em.getComponent<T>(id);
        };
        deserializers[name] = [this](EntityId id, const json& j) {
            T* existing = em.getComponent<T>(id);
            if (existing) {
                *existing = j.get<T>();
            } else {
                em.addComponent<T>(id, new T(j.get<T>()));
            }
        };
        checkers[name] = [this](EntityId id) -> bool {
            return em.hasComponent<T>(id);
        };
        if (inspect) {
            inspectors[name] = [this, inspect](EntityId id) {
                T* comp = em.getComponent<T>(id);
                if (comp) inspect(id, *comp);
            };
        }
        defaults[name] = [this](EntityId id) {
            em.addComponent<T>(id, new T());
        };
        removers[name] = [this](EntityId id) {
            em.removeComponent<T>(id);
        };
        componentNames.push_back(name);
    }

    json serialize(const std::string& name, EntityId id) {
        auto it = serializers.find(name);
        if (it == serializers.end()) return nullptr;
        json j;
        j["componentName"] = name;
        j["properties"] = it->second(id);
        return j;
    }

    void deserialize(const std::string& name, EntityId id, const json& properties) {
        auto it = deserializers.find(name);
        if (it == deserializers.end()) return;
        it->second(id, properties);
    }

    void inspect(const std::string& name, EntityId id) {
        auto it = inspectors.find(name);
        if (it != inspectors.end()) it->second(id);
    }

    void addDefault(const std::string& name, EntityId id) {
        auto it = defaults.find(name);
        if (it != defaults.end()) it->second(id);
    }

    void removeComponent(const std::string& name, EntityId id) {
        auto it = removers.find(name);
        if (it != removers.end()) it->second(id);
    }

    bool hasComponent(const std::string& name, EntityId id) {
        auto it = checkers.find(name);
        if (it == checkers.end()) return false;
        return it->second(id);
    }

    std::vector<std::string> getComponentNames(EntityId id) {
        std::vector<std::string> result;
        for (auto& name : componentNames) {
            auto it = checkers.find(name);
            if (it != checkers.end() && it->second(id)) {
                result.push_back(name);
            }
        }
        return result;
    }

    std::vector<std::string>& getAllComponentNames() {
        return componentNames;
    }

    void registerAllComponents() {
        registerComponent<components::AudioComponent>("AudioComponent",
        [](EntityId id, components::AudioComponent& comp) {
            char buf[256];
            strncpy(buf, comp.filePath.c_str(), sizeof(buf));
            if (ImGui::InputText("File Path", buf, sizeof(buf)))
                comp.filePath = buf;
            ImGui::SliderInt("Volume", &comp.volume, 0, 128);
            ImGui::InputInt("Num Loops", &comp.numLoops);
            ImGui::Checkbox("Is Music?", &comp.isMusic);
        });

        registerComponent<components::Camera>("Camera",
            [](EntityId id, components::Camera& comp) {
                ImGui::DragFloat2("Position", &comp.position.x);
                ImGui::DragFloat2("Follow Offset", &comp.followOffset.x);
                ImGui::InputInt("Follow Entity", (int*)&comp.follow);
                ImGui::Checkbox("Should Lerp", &comp.shouldLerp);
                ImGui::DragFloat("Smoothing Factor", &comp.smoothingFactor, 0.1f, 0.1f, 20.0f);
            });

        registerComponent<components::ColliderComponent>("ColliderComponent",
            [](EntityId id, components::ColliderComponent& comp) {
                ImGui::DragInt4("Rect", &comp.rect.x);
                float color[4] = {
                    comp.color.r / 255.0f,
                    comp.color.g / 255.0f,
                    comp.color.b / 255.0f,
                    comp.color.a / 255.0f
                };
                if (ImGui::ColorEdit4("Color", color)) {
                    comp.color.r = (Uint8)(color[0] * 255);
                    comp.color.g = (Uint8)(color[1] * 255);
                    comp.color.b = (Uint8)(color[2] * 255);
                    comp.color.a = (Uint8)(color[3] * 255);
                }
                ImGui::DragFloat2("Scale", &comp.scale.x);
                ImGui::Checkbox("Show Outline", &comp.showOutline);
                ImGui::Checkbox("Encompass Transform", &comp.encompassTransform);
                ImGui::Checkbox("Is Trigger", &comp.isTrigger);
                // ignore list
                ImGui::Text("Ignore List (%zu)", comp.ignore.size());
                for (int i = 0; i < (int)comp.ignore.size(); i++) {
                    ImGui::PushID(i);
                    ImGui::InputInt("##ignore", (int*)&comp.ignore[i]);
                    ImGui::SameLine();
                    if (ImGui::SmallButton("X")) {
                        comp.ignore.erase(comp.ignore.begin() + i);
                        ImGui::PopID();
                        break;
                    }
                    ImGui::PopID();
                }
                if (ImGui::SmallButton("+ Add Ignore")) {
                    comp.ignore.push_back(0);
                }
            });

        registerComponent<components::MeshComponent>("MeshComponent",
            [](EntityId id, components::MeshComponent& comp) {
                char buf[256];
                strncpy(buf, comp.imagePath.c_str(), sizeof(buf));
                if (ImGui::InputText("Image Path", buf, sizeof(buf)))
                    comp.imagePath = buf;
                ImGui::DragInt4("Rect",   &comp.rect.x);
                float color[4] = {
                    comp.color.r / 255.0f,
                    comp.color.g / 255.0f,
                    comp.color.b / 255.0f,
                    comp.color.a / 255.0f
                };
                if (ImGui::ColorEdit4("Color", color)) {
                    comp.color.r = (Uint8)(color[0] * 255);
                    comp.color.g = (Uint8)(color[1] * 255);
                    comp.color.b = (Uint8)(color[2] * 255);
                    comp.color.a = (Uint8)(color[3] * 255);
                }
                ImGui::Checkbox("Flip X?",&comp.flipX);
                ImGui::Checkbox("Flip Y?",&comp.flipY);
                ImGui::InputInt("Z Index",&comp.zIndex, 1, 10);
            });

        registerComponent<components::PanelGuiComponent>("PanelGuiComponent",
            [](EntityId id, components::PanelGuiComponent& comp) {
                float color[4] = {
                    comp.color.r / 255.0f,
                    comp.color.g / 255.0f,
                    comp.color.b / 255.0f,
                    comp.color.a / 255.0f
                };
                if (ImGui::ColorEdit4("Color", color)) {
                    comp.color.r = (Uint8)(color[0] * 255);
                    comp.color.g = (Uint8)(color[1] * 255);
                    comp.color.b = (Uint8)(color[2] * 255);
                    comp.color.a = (Uint8)(color[3] * 255);
                }
                ImGui::Checkbox("In World Space", &comp.inWorldSpace);
                ImGui::InputInt("Z Index", &comp.zIndex, 1, 10);
            });

        registerComponent<components::RigidBodyComponent>("RigidBodyComponent",
            [](EntityId id, components::RigidBodyComponent& comp) {
                ImGui::DragFloat2("Velocity", &comp.velocity.x);
                ImGui::DragFloat("Max Velocity", &comp.maxVelocity, 1.0f, 0.0f, 5000.0f);
                ImGui::Checkbox("Use Gravity", &comp.useGravity);
            });

        registerComponent<components::TextGuiComponent>("TextGuiComponent",
            [](EntityId id, components::TextGuiComponent& comp) {
                char textBuf[512];
                strncpy(textBuf, comp.text.c_str(), sizeof(textBuf));
                if (ImGui::InputText("Text", textBuf, sizeof(textBuf)))
                    comp.text = textBuf;
                char fontBuf[256];
                strncpy(fontBuf, comp.fontPath.c_str(), sizeof(fontBuf));
                if (ImGui::InputText("Font Path", fontBuf, sizeof(fontBuf)))
                    comp.fontPath = fontBuf;
                ImGui::InputInt("Text Size", &comp.textSize, 1, 10);
                float color[4] = {
                    comp.textColor.r / 255.0f,
                    comp.textColor.g / 255.0f,
                    comp.textColor.b / 255.0f,
                    comp.textColor.a / 255.0f
                };
                if (ImGui::ColorEdit4("Text Color", color)) {
                    comp.textColor.r = (Uint8)(color[0] * 255);
                    comp.textColor.g = (Uint8)(color[1] * 255);
                    comp.textColor.b = (Uint8)(color[2] * 255);
                    comp.textColor.a = (Uint8)(color[3] * 255);
                }
                ImGui::Checkbox("In World Space", &comp.inWorldSpace);
                ImGui::InputInt("Z Index", &comp.zIndex, 1, 10);
                ImGui::DragInt2("Texture Scale", &comp.textureScale.x);
            });

        registerComponent<components::TransformComponent>("TransformComponent",
            [](EntityId id, components::TransformComponent& comp) {
                ImGui::DragFloat2("Position", &comp.position.x);
                ImGui::DragFloat2("Scale", &comp.scale.x);
                ImGui::DragFloat("Rotation", &comp.rotation);
                ImGui::DragInt2("Anchor", &comp.anchor.x);
            });

        registerComponent<components::WeldComponent>("WeldComponent",
            [](EntityId id, components::WeldComponent& comp) {
                ImGui::Text("Welds (%zu)", comp.welds.size());
                for (int i = 0; i < (int)comp.welds.size(); i++) {
                    ImGui::PushID(i);
                    auto& w = comp.welds[i];
                    ImGui::InputInt("Child Entity", (int*)&w.child);
                    ImGui::DragFloat2("Position Offset", &w.positionOffset.x);
                    ImGui::Checkbox("Follow Rotation", &w.followRotation);
                    ImGui::SameLine();
                    if (ImGui::SmallButton("X")) {
                        comp.welds.erase(comp.welds.begin() + i);
                        ImGui::PopID();
                        break;
                    }
                    ImGui::Separator();
                    ImGui::PopID();
                }
                if (ImGui::SmallButton("+ Add Weld")) {
                    comp.welds.push_back({});
                }
            });
    }

private:
    ComponentRegistry() = default;
    ~ComponentRegistry() = default;
    ComponentRegistry(const ComponentRegistry&) = delete;
    ComponentRegistry& operator=(const ComponentRegistry&) = delete;

    std::unordered_map<std::string, SerializeFn> serializers;
    std::unordered_map<std::string, DeserializeFn> deserializers;
    std::unordered_map<std::string, CheckerFn> checkers;
    std::unordered_map<std::string, InspectFn> inspectors;
    std::unordered_map<std::string, DefaultFn> defaults;
    std::unordered_map<std::string, RemoveFn> removers;
    std::vector<std::string> componentNames;

    simpleengine2d::core::EntityManager &em = simpleengine2d::core::EntityManager::getInstance();
};

}
