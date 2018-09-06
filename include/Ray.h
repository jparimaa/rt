#pragma once

#include <glm/glm.hpp>

class Ray
{
public:
    Ray() {}
    Ray(const glm::vec3& origin, const glm::vec3& direction);

    glm::vec3 origin() const;
    glm::vec3 direction() const;
    glm::vec3 pointAt(float t) const;

private:
    glm::vec3 m_origin;
    glm::vec3 m_direction;
};