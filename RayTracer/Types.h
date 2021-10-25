#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtx/color_space.hpp>

#include <sdl.h>

using color_t = SDL_Color;

inline color_t Vec3ToColor(const glm::vec3& vec3)
{
    color_t color;

    color.r = static_cast<std::uint8_t>(glm::clamp(vec3[0], 0.0f, 1.0f) * 255);
    color.g = static_cast<std::uint8_t>(glm::clamp(vec3[1], 0.0f, 1.0f) * 255);
    color.b = static_cast<std::uint8_t>(glm::clamp(vec3[2], 0.0f, 1.0f) * 255);
    color.a = 0;

    return color;
}

inline float dot(const glm::vec3& v1, const glm::vec3& v2)
{
    return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

inline float angle(const glm::vec3& v1, const glm::vec3& v2)
{
    return glm::acos(dot(v1, v2));
}