#pragma once
#include <glm/ext/vector_float2.hpp>
#include <imgui.h>
#include <glm/glm.hpp>
#include "SimpleEngine2D/components/Transform.hpp"

namespace simpleengine2d::editor {

ImVec2 WorldToScreen(glm::vec2 worldPos, ImVec2 viewportPos);
void DrawTranslateGizmo(components::TransformComponent &transform, ImVec2 viewportPos);
void DrawScaleGizmo(components::TransformComponent &transform, ImVec2 viewportPos);
void DrawRotationGizmo(components::TransformComponent &transform, ImVec2 viewportPos);

const float LINE_LENGTH = 75.0f;
const float LINE_THICKNESS = 4.0f;
const float HOVER_RADIUS = 10.0f;
const float SCALE_HANDLE_SIZE = 8.0f;
const float ROTATION_RADIUS = 60.0f;

const ImU32 COLOR_X = IM_COL32(220, 50, 50, 255);
const ImU32 COLOR_Y = IM_COL32(50, 220, 50, 255);
const ImU32 COLOR_ROTATION = IM_COL32(50, 50, 220, 255);
const ImU32 COLOR_X_HOVER = IM_COL32(150, 20, 20, 255);
const ImU32 COLOR_Y_HOVER = IM_COL32(20, 150, 20, 255);
const ImU32 COLOR_ROTATION_HOVER = IM_COL32(20, 20, 150, 255);

}