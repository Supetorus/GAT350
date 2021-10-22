#pragma once
#include "Types.h"

struct Ray
{
	glm::vec3 origin;
	glm::vec3 direction;

	Ray() = default; //same as using empty brackets to define empty constructor

	Ray(const glm::vec3& origin, const glm::vec3& direction) : origin{ origin }, direction{ direction } {}

	glm::vec3 pointAt(float t) const { return origin + direction * t; }
};