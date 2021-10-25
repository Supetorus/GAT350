#include "Sphere.h"

bool Sphere::Hit(const ray_t& r, float tMin, float tMax, raycastHit_t& hit)
{
	glm::vec3 oc = r.origin - center;

	float a = dot(r.direction, r.direction);
	float b = 2 * dot(r.direction, oc);
	float c = dot(oc, oc) - (radius * radius);
	float discriminant = (b * b) - (4 * a * c);

	return discriminant >= 0;
}