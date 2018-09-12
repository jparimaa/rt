#include "Camera.h"
#include "Common.h"

#include <cmath>

Camera::Camera(glm::vec3 position, glm::vec3 lookAt, glm::vec3 worldUp, float fov, float aspectRatio, float aperture, float focusDistance) :
    m_origin(position),
    m_lendRadius(aperture / 2.0f)
{
    float halfWidth = tan(fov / 2.0f);
    float halfHeight = halfWidth / aspectRatio;
    m_forward = glm::normalize(lookAt - position);
    m_right = glm::normalize(glm::cross(m_forward, worldUp));
    m_up = glm::cross(m_right, m_forward);
    m_lowerLeftCorner = m_origin - halfWidth * m_right * focusDistance - halfHeight * m_up * focusDistance + m_forward * focusDistance;
    m_horizontal = 2.0f * halfWidth * m_right * focusDistance;
    m_vertical = 2.0f * halfHeight * m_up * focusDistance;
}

Ray Camera::getRay(float u, float v) const
{
    glm::vec3 random = m_lendRadius * randomInUnitSphere();
    random.z = 0.0f;
    glm::vec3 offset = m_right * random.x + m_up * random.y;
    glm::vec3 direction(m_lowerLeftCorner + u * m_horizontal + v * m_vertical - m_origin - offset);
    Ray ray(m_origin + offset, direction);
    return ray;
}
