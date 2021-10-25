#include "Tracer.h"
#include "ColorBuffer.h"
#include "Scene.h"

void Tracer::Trace(const ColorBuffer& colorBuffer, Scene* scene)
{
	for (int y = 0; y < colorBuffer.height; y++)
	{
		for (int x = 0; x < colorBuffer.width; x++)
		{
			glm::vec2 point;
			point.x = x / (float)colorBuffer.width;
			point.y = 1.0f - y / (float)colorBuffer.height;

			glm::vec3 direction = glm::vec3{ point * 2.0f - 1.0f, 1 };
			direction = glm::normalize(direction);

			ray_t ray{ {0, 0, 0}, direction };

			raycastHit_t hit;
			glm::vec3 color = scene->Trace(ray, 0.001f, FLT_MAX, hit);

			colorBuffer.SetColor(x, y, Vec3ToColor(color));
		}
	}
}
