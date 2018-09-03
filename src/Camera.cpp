#include "Camera.h"

Camera::Camera()
{
}

Ray Camera::getRay(float u, float v)
{
    glm::vec3 direction(m_lowerLeftCorner + u * m_horizontal + v * m_vertical);
    Ray ray(m_origin, direction);
    return ray;
}
