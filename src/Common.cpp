#include "Common.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/norm.hpp>

#include <random>

glm::vec3 randomInUnitSphere()
{
    static std::default_random_engine random;
    static std::uniform_real_distribution<> distribution(0, 1); // rage 0 - 1
    glm::vec3 p;
    do
    {
        // 2.0f * x - 1.0f to make it between [-1..1)
        p = 2.0f * glm::vec3(distribution(random), distribution(random), distribution(random)) - glm::vec3(1.0f);
    } while (glm::length2(p) >= 1.0f); // Squared length is faster and does the same thing in this situation
    return p;
}