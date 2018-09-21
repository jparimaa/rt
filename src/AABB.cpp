#include "AABB.h"

#include <utility>
#include <algorithm>

AABB::AABB(const glm::vec3& min, const glm::vec3& max) :
    m_min(min),
    m_max(max)
{
}

AABB::AABB(const AABB& box1, const AABB& box2)
{
    glm::vec3 box1min = box1.getMin();
    glm::vec3 box2min = box2.getMin();
    glm::vec3 box1max = box1.getMax();
    glm::vec3 box2max = box2.getMax();
    m_min = glm::vec3(std::min(box1min.x, box2min.x),
                      std::min(box1min.y, box2min.y),
                      std::min(box1min.z, box2min.z));
    m_max = glm::vec3(std::max(box1max.x, box2max.x),
                      std::max(box1max.y, box2max.y),
                      std::max(box1max.z, box2max.z));
}

bool AABB::hit(const Ray& ray, float tmin, float tmax)
{
    for (int a = 0; a < 3; ++a)
    {
        float inverted = 1.0f / ray.direction()[a];
        float t0 = (m_min[a] - ray.origin()[a]) * inverted;
        float t1 = (m_max[a] - ray.origin()[a]) * inverted;
        if (inverted < 0.0f)
        {
            std::swap(t0, t1);
        }

        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;
        if (tmax <= tmin)
        {
            return false;
        }
    }
    return true;
}

glm::vec3 AABB::getMin() const
{
    return m_min;
}

glm::vec3 AABB::getMax() const
{
    return m_max;
}
