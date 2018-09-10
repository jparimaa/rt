#pragma once

#include "Ray.h"

#include <glm/glm.hpp>

class Camera
{
public:
    Camera(glm::vec3 position, glm::vec3 lookAt, glm::vec3 up, float fov, float aspectRatio);
    Ray getRay(float u, float v) const;

private:
    glm::vec3 m_origin = {0.0f, 0.0f, 0.0f};
    glm::vec3 m_lowerLeftCorner;
    glm::vec3 m_horizontal;
    glm::vec3 m_vertical;
};