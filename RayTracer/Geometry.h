#pragma once
#include "Types.h"
#include "Ray.h"
#include "Material.h"

class Geometry
{
public:
    Geometry() = default;
    virtual ~Geometry() = default;
    Geometry(std::shared_ptr<Material> material) : material{ material } {}

    virtual bool Hit(const ray_t& r, float tMin, float tMax, raycastHit_t& hit) = 0;
public:
    std::shared_ptr<Material> material;
};