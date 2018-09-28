#pragma once

#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace Constants
{
const int width = 640;
const int height = 480;
const int numSamples = 300;
const int maxDepth = 25;
const int numThreads = 10;
const int totalImageSize = width * height * 3;
const float maxDistance = std::numeric_limits<float>::max();

const glm::vec3 position(0.0f, 2.5f, 0.0f);
const glm::vec3 lookAt(0.0f, 0.0f, -10.0f);
const glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
const float fov = glm::quarter_pi<float>();
const float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
const float aperture = 0.10f;
const float focusDistance = glm::distance(lookAt, position);
const Camera camera(position, lookAt, worldUp, fov, aspectRatio, aperture, focusDistance);
}