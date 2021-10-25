#include "Scene.h"

glm::vec3 Scene::Trace(const ray_t& ray, float tMin, float tMax, raycastHit_t& hit)
{
    float tClosest = tMax;
    bool rayHit = false;
    for (auto& object : objects)
    {
        if (object->Hit(ray, tMin, tClosest, hit))
        {
            rayHit = true;
            tClosest = hit.t;
        }
    }

    if (rayHit)
    {
        return glm::vec3{ 1, 0, 0 };
    }

    return glm::vec3{ 0, 0, 0 };
}

void Scene::Add(std::unique_ptr<Geometry> geometry)
{
	objects.push_back(std::move(geometry));
}
