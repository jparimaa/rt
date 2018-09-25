#pragma once

#include "Ray.h"
#include "AABB.h"

#include <glm/glm.hpp>

class Material;
class Hitable;

struct Hit
{
    float t;
    glm::vec3 p;
    glm::vec3 normal;
    const Material* material;
    const Hitable* hitable;
};

class Hitable
{
public:
    virtual ~Hitable(){};
    virtual bool hit(const Ray& r, float min, float max, Hit& hit) const = 0;
    virtual bool getBoundingBox(AABB& box) = 0;
    virtual glm::vec2 getUV(glm::vec3 p) const = 0;
};
