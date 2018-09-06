#pragma once

#include "Ray.h"

#include <glm/glm.hpp>

struct HitRecord
{
    float t;
    glm::vec3 p;
    glm::vec3 normal;
};

class Hitable
{
public:
    virtual bool hit(const Ray& r, float min, float max, HitRecord& record) const = 0;
};
