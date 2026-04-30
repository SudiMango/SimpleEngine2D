#pragma once
#include <nlohmann/json.hpp>
#include <SDL2/SDL.h>

using json = nlohmann::json;

inline void to_json(json &j, const SDL_Point &p) { j = {p.x, p.y}; }
inline void from_json(const json &j, SDL_Point &p) { p.x = j[0]; p.y = j[1]; }

inline void to_json(json &j, const SDL_Rect &r) { j = {r.x, r.y, r.w, r.h}; }
inline void from_json(const json &j, SDL_Rect &r) { r.x = j[0]; r.y = j[1]; r.w = j[2]; r.h = j[3]; }

inline void to_json(json &j, const SDL_Color &c) { j = {c.r, c.g, c.b, c.a}; }
inline void from_json(const json &j, SDL_Color &c) { c.r = j[0]; c.g = j[1]; c.b = j[2]; c.a = j[3]; }