#include "Ray.h"

Ray::Ray(const glm::vec3& a, const glm::vec3& b) :
    m_A(a),
    m_B(b)
{
}

glm::vec3 Ray::origin() const
{
    return m_A;
}

glm::vec3 Ray::direction() const
{
    return m_B;
}

glm::vec3 Ray::pointAt(float t) const
{
    return m_A + t * m_B;
}
