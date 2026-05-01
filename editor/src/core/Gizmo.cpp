#include "core/Gizmo.hpp"
#include "SimpleEngine2D/core/Engine.hpp"

namespace simpleengine2d::editor {

ImVec2 WorldToScreen(glm::vec2 worldPos, ImVec2 viewportPos) {
    core::Engine &engine = core::Engine::getInstance();
    return ImVec2(
        worldPos.x - engine.getCamera()->position.x + viewportPos.x,
        worldPos.y - engine.getCamera()->position.y + viewportPos.y
    );
}

void DrawTranslateGizmo(components::TransformComponent &transform, ImVec2 viewportPos) {
    ImDrawList* dl = ImGui::GetWindowDrawList();

    glm::vec2 worldAnchorPos = glm::vec2(
        transform.position.x + transform.anchor.x,
        transform.position.y + transform.anchor.y
    );
    ImVec2 origin = WorldToScreen(worldAnchorPos, viewportPos);

    ImVec2 xEnd = ImVec2(origin.x + LINE_LENGTH, origin.y);
    ImVec2 yEnd = ImVec2(origin.x, origin.y - LINE_LENGTH);

    ImVec2 mousePos = ImGui::GetMousePos();

    auto distToSegment = [](ImVec2 p, ImVec2 a, ImVec2 b) -> float {
        float dx = b.x - a.x, dy = b.y - a.y;
        float lenSq = dx*dx + dy*dy;
        if (lenSq == 0.0f) return hypotf(p.x - a.x, p.y - a.y);
        float t = ((p.x - a.x)*dx + (p.y - a.y)*dy) / lenSq;
        t = fmaxf(0.0f, fminf(1.0f, t));
        return hypotf(p.x - (a.x + t*dx), p.y - (a.y + t*dy));
    };

    bool hoverX = distToSegment(mousePos, origin, xEnd) < HOVER_RADIUS;
    bool hoverY = distToSegment(mousePos, origin, yEnd) < HOVER_RADIUS;

    static bool draggingX = false;
    static bool draggingY = false;

    if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        draggingX = false;
        draggingY = false;
    }

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        if (hoverX) draggingX = true;
        if (hoverY) draggingY = true;
    }

    if (draggingX || draggingY) {
        ImVec2 delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left, 0.0f);
        ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
        if (draggingX) transform.position.x += delta.x;
        if (draggingY) transform.position.y += delta.y;
    }

    auto drawArrow = [&](ImVec2 from, ImVec2 to, ImU32 color) {
        dl->AddLine(from, to, color, LINE_THICKNESS);
    };

    ImU32 currentXColor = (hoverX || draggingX) ? COLOR_X_HOVER : COLOR_X;
    ImU32 currentYColor = (hoverY || draggingY) ? COLOR_Y_HOVER : COLOR_Y;

    drawArrow(origin, xEnd, currentXColor);
    drawArrow(origin, yEnd, currentYColor);

    dl->AddCircleFilled(origin, 4.0f, IM_COL32(255, 255, 255, 200));
}

void DrawScaleGizmo(components::TransformComponent &transform, ImVec2 viewportPos) {
    ImDrawList* dl = ImGui::GetWindowDrawList();

    glm::vec2 worldAnchorPos = glm::vec2(
        transform.position.x + transform.anchor.x,
        transform.position.y + transform.anchor.y
    );
    ImVec2 origin = WorldToScreen(worldAnchorPos, viewportPos);

    ImVec2 xEnd = ImVec2(origin.x + LINE_LENGTH, origin.y);
    ImVec2 yEnd = ImVec2(origin.x, origin.y - LINE_LENGTH);

    ImVec2 mousePos = ImGui::GetMousePos();

    auto distToSegment = [](ImVec2 p, ImVec2 a, ImVec2 b) -> float {
        float dx = b.x - a.x, dy = b.y - a.y;
        float lenSq = dx*dx + dy*dy;
        if (lenSq == 0.0f) return hypotf(p.x - a.x, p.y - a.y);
        float t = ((p.x - a.x)*dx + (p.y - a.y)*dy) / lenSq;
        t = fmaxf(0.0f, fminf(1.0f, t));
        return hypotf(p.x - (a.x + t*dx), p.y - (a.y + t*dy));
    };

    bool hoverX = distToSegment(mousePos, origin, xEnd) < HOVER_RADIUS;
    bool hoverY = distToSegment(mousePos, origin, yEnd) < HOVER_RADIUS;

    static bool draggingX = false;
    static bool draggingY = false;

    if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        draggingX = false;
        draggingY = false;
    }

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
        if (hoverX) draggingX = true;
        if (hoverY) draggingY = true;
    }

    if (draggingX || draggingY) {
        ImVec2 delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Left, 0.0f);
        ImGui::ResetMouseDragDelta(ImGuiMouseButton_Left);
        if (draggingX) transform.scale.x += delta.x;
        if (draggingY) transform.scale.y += delta.y;
    }

    auto drawArrow = [&](ImVec2 from, ImVec2 to, ImU32 color) {
        dl->AddLine(from, to, color, LINE_THICKNESS);

        ImVec2 rectMin = ImVec2(to.x - (SCALE_HANDLE_SIZE / 2.0f), to.y - (SCALE_HANDLE_SIZE / 2.0f));
        ImVec2 rectMax = ImVec2(to.x + (SCALE_HANDLE_SIZE / 2.0f), to.y + (SCALE_HANDLE_SIZE / 2.0f));
        dl->AddRectFilled(rectMin, rectMax, color);
    };

    ImU32 currentXColor = (hoverX || draggingX) ? COLOR_X_HOVER : COLOR_X;
    ImU32 currentYColor = (hoverY || draggingY) ? COLOR_Y_HOVER : COLOR_Y;

    drawArrow(origin, xEnd, currentXColor);
    drawArrow(origin, yEnd, currentYColor);

    dl->AddCircleFilled(origin, 4.0f, IM_COL32(255, 255, 255, 200));
}

void DrawRotationGizmo(components::TransformComponent &transform, ImVec2 viewportPos) {
    ImDrawList* dl = ImGui::GetWindowDrawList();

    glm::vec2 worldAnchorPos = glm::vec2(
        transform.position.x + transform.anchor.x,
        transform.position.y + transform.anchor.y
    );
    ImVec2 origin = WorldToScreen(worldAnchorPos, viewportPos);
    ImVec2 mousePos = ImGui::GetMousePos();

    float distToMouse = hypotf(mousePos.x - origin.x, mousePos.y - origin.y);
    bool hover = fabsf(distToMouse - ROTATION_RADIUS) < HOVER_RADIUS;

    static bool dragging = false;
    static float startRotation = 0.0f;
    static float startMouseAngle = 0.0f;

    if (!ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        dragging = false;
    }

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && hover) {
        dragging = true;
        startMouseAngle = atan2f(mousePos.y - origin.y, mousePos.x - origin.x);
    }

    if (dragging) {
        float currentMouseAngle = atan2f(mousePos.y - origin.y, mousePos.x - origin.x);

        float angleDelta = currentMouseAngle - startMouseAngle;
        if (angleDelta > M_PI) angleDelta -= 2.0f * M_PI;
        if (angleDelta < -M_PI) angleDelta += 2.0f * M_PI;

        transform.rotation += angleDelta * (180.0f / M_PI);
        startMouseAngle = currentMouseAngle;
    }

    ImU32 color = (hover || dragging) ? COLOR_ROTATION_HOVER : COLOR_ROTATION;
    dl->AddCircle(origin, ROTATION_RADIUS, color, 32, LINE_THICKNESS);

    if (dragging) {
        ImVec2 lineEnd = ImVec2(
            origin.x + cosf(atan2f(mousePos.y - origin.y, mousePos.x - origin.x)) * ROTATION_RADIUS,
            origin.y + sinf(atan2f(mousePos.y - origin.y, mousePos.x - origin.x)) * ROTATION_RADIUS
        );
        dl->AddLine(origin, lineEnd, color, 1.0f);
    }

    dl->AddCircleFilled(origin, 4.0f, IM_COL32(255, 255, 255, 200));
}

}