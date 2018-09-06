#pragma once

#include "Ray.h"

#include <glm/glm.hpp>

class Camera
{
public:
    Camera();
    Ray getRay(float u, float v) const;

private:
    glm::vec3 m_origin = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_lowerLeftCorner = {-2.0f, -1.0f, -1.0f};
    glm::vec3 m_horizontal = {4.0f, 0.0f, 0.0f};
    glm::vec3 m_vertical = {0.0f, 2.0f, 0.0f};
};