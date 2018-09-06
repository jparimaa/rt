#include "Ray.h"

Ray::Ray(const glm::vec3& origin, const glm::vec3& direction) :
    m_origin(origin),
    m_direction(direction)
{
}

glm::vec3 Ray::origin() const
{
    return m_origin;
}

glm::vec3 Ray::direction() const
{
    return m_direction;
}

glm::vec3 Ray::pointAt(float t) const
{
    return m_origin + t * m_direction;
}
