#pragma once

#include "Ray.h"
#include "AABB.h"

#include <glm/glm.hpp>

class Material;

struct Hit
{
    float t;
    glm::vec3 p;
    glm::vec3 normal;
    Material* material;
};

class Hitable
{
public:
    virtual ~Hitable(){};
    virtual bool hit(const Ray& r, float min, float max, Hit& hit) const = 0;
    virtual bool getBoundingBox(AABB& box) = 0;
};
