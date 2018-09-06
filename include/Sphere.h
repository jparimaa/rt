#pragma once

#include "Hitable.h"
#include "Ray.h"

#include <glm/glm.hpp>

class Sphere : public Hitable
{
public:
    Sphere(glm::vec3 center, float radius);

    virtual bool hit(const Ray& ray, float min, float max, HitRecord& record) const;

private:
    glm::vec3 m_center;
    float m_radius;
};
