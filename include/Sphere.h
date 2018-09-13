#pragma once

#include "Hitable.h"
#include "Ray.h"

#include <glm/glm.hpp>

class Sphere : public Hitable
{
public:
    Sphere(glm::vec3 center, float radius, Material* material);
    virtual ~Sphere() {};

    virtual bool hit(const Ray& ray, float min, float max, Hit& hit) const;

private:
    glm::vec3 m_center;
    float m_radius;
    Material* m_material;
};
