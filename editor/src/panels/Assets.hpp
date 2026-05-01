#pragma once

#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>
#include <SDL2/SDL.h>
#include "imgui.h"
#include "core/Panel.hpp"

namespace simpleengine2d::editor {

class Assets : public Panel {
public:
    void init() override {
        rootPath = std::filesystem::path("./games/test-game");
    }

    void update(float dt) override {
        ImGui::Begin("Assets");

        ImGui::Text("Project Root");
        ImGui::SameLine();
        ImGui::TextDisabled("%s", rootPath.string().c_str());
        ImGui::Separator();

        if (!std::filesystem::exists(rootPath)) {
            ImGui::TextDisabled("Path not found: %s", rootPath.string().c_str());
            ImGui::End();
            return;
        }

        drawNode(rootPath);

        ImGui::End();
    }

    void clean() override {}

private:
    std::filesystem::path rootPath = {};
    std::filesystem::path selectedPath = {};

    void drawNode(const std::filesystem::path &path) {
        const std::string label = path.filename().empty() ? path.string() : path.filename().string();
        const bool isDir = std::filesystem::is_directory(path);
        const bool isSelected = (selectedPath == path);

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
        if (isSelected) flags |= ImGuiTreeNodeFlags_Selected;
        if (!isDir) flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        const bool opened = ImGui::TreeNodeEx(path.string().c_str(), flags, "%s", label.c_str());

        if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen()) {
            selectedPath = path;
        }

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && !isDir) {
            openExternally(path);
        }

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Open")) {
                openExternally(path);
            }
            ImGui::EndPopup();
        }

        if (!isDir || !opened) {
            return;
        }

        std::error_code ec;
        std::vector<std::filesystem::directory_entry> entries;
        for (const auto &entry : std::filesystem::directory_iterator(path, ec)) {
            if (!ec) {
                entries.push_back(entry);
            }
        }

        std::sort(entries.begin(), entries.end(), [](const auto &a, const auto &b) {
            const bool aIsDir = a.is_directory();
            const bool bIsDir = b.is_directory();
            if (aIsDir != bIsDir) return aIsDir > bIsDir;
            return a.path().filename().string() < b.path().filename().string();
        });

        for (const auto &entry : entries) {
            drawNode(entry.path());
        }

        ImGui::TreePop();
    }

    bool isWithinRoot(const std::filesystem::path &path) const {
        const auto absRoot = std::filesystem::weakly_canonical(rootPath);
        const auto absPath = std::filesystem::weakly_canonical(path);

        auto [rootEnd, nothing] = std::mismatch(absRoot.begin(), absRoot.end(), absPath.begin());
        return rootEnd == absRoot.end();
    }

    void openExternally(const std::filesystem::path &path) {
        if (!isWithinRoot(path)) return;

        const std::filesystem::path absolutePath = std::filesystem::absolute(path);

        #if defined(_WIN32)
            const std::string cmd = "explorer \"" + absolutePath.string() + "\"";
            system(cmd.c_str());
        #elif defined(__APPLE__)
            const std::string cmd = "open \"" + absolutePath.string() + "\"";
            system(cmd.c_str());
        #else
            const std::string cmd = "xdg-open \"" + absolutePath.string() + "\" &";
            system(cmd.c_str());
        #endif
    }
};

}
