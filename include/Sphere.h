#pragma once

#include "Hitable.h"
#include "AABB.h"
#include "Ray.h"

#include <glm/glm.hpp>

class Sphere : public Hitable
{
public:
    Sphere(glm::vec3 center, float radius, Material* material);
    virtual ~Sphere(){};

    virtual bool hit(const Ray& ray, float min, float max, Hit& hit) const;
    virtual bool getBoundingBox(AABB& box);
    virtual glm::vec2 getUV(glm::vec3 p) const override;

private:
    glm::vec3 m_center;
    float m_radius;
    Material* m_material;
};
