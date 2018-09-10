#include "Camera.h"

#include <cmath>

Camera::Camera(glm::vec3 position, glm::vec3 lookAt, glm::vec3 worldUp, float fov, float aspectRatio) :
    m_origin(position)
{
    float halfWidth = tan(fov / 2.0f);
    float halfHeight = halfWidth / aspectRatio;
    glm::vec3 forward = glm::normalize(lookAt - position);
    glm::vec3 right = glm::normalize(glm::cross(forward, worldUp));
    glm::vec3 up = glm::cross(right, forward);
    m_lowerLeftCorner = m_origin - halfWidth * right - halfHeight * up + forward;
    m_horizontal = 2.0f * halfWidth * right;
    m_vertical = 2.0f * halfHeight * up;
}

Ray Camera::getRay(float u, float v) const
{
    glm::vec3 direction(m_lowerLeftCorner + u * m_horizontal + v * m_vertical - m_origin);
    Ray ray(m_origin, direction);
    return ray;
}
