#pragma once

#include "Material.h"

#include <glm/glm.hpp>

#include <vector>

class SceneCreator
{
public:
    struct SphereData
    {
        glm::vec3 position;
        float radius;
        Material::Type type;
        glm::vec3 color;
        float fuzziness;
        float refractionIndex;
        std::string texture;
    };

    static void createSphereDataset(std::vector<SceneCreator::SphereData>& dataset, int numBalls);

    SceneCreator() = delete;
};