#pragma once
#include <nlohmann/json.hpp>
#include <glm/glm.hpp>

using json = nlohmann::json;

namespace glm {

inline void to_json(json &j, const vec2 &v) { j = {v.x, v.y}; }
inline void from_json(const json &j, vec2 &v) { v.x = j[0]; v.y = j[1]; }

inline void to_json(json &j, const vec3 &v) { j = {v.x, v.y, v.z}; }
inline void from_json(const json &j, vec3 &v) { v.x = j[0]; v.y = j[1]; v.z = j[2]; }

inline void to_json(json &j, const vec4 &v) { j = {v.x, v.y, v.z, v.w}; }
inline void from_json(const json &j, vec4 &v) { v.x = j[0]; v.y = j[1]; v.z = j[2]; v.w = j[3]; }

inline void to_json(json &j, const ivec2 &v) { j = {v.x, v.y}; }
inline void from_json(const json &j, ivec2 &v) { v.x = j[0]; v.y = j[1]; }

inline void to_json(json &j, const ivec3 &v) { j = {v.x, v.y, v.z}; }
inline void from_json(const json &j, ivec3 &v) { v.x = j[0]; v.y = j[1]; v.z = j[2]; }

inline void to_json(json &j, const ivec4 &v) { j = {v.x, v.y, v.z, v.w}; }
inline void from_json(const json &j, ivec4 &v) { v.x = j[0]; v.y = j[1]; v.z = j[2]; v.w = j[3]; }

}