#pragma once

#include "Ray.h"

#include <glm/glm.hpp>

class AABB
{
public:
    AABB() {}
    AABB(const glm::vec3& min, const glm::vec3& max);
    AABB(const AABB& box1, const AABB& box2);

    bool hit(const Ray& ray, float tmin, float tmax) const;

    glm::vec3 getMin() const;
    glm::vec3 getMax() const;

private:
    glm::vec3 m_min;
    glm::vec3 m_max;
};