#include "SceneCreator.h"
#include "Constants.h"

#include <random>

namespace
{
std::default_random_engine l_random;
std::uniform_real_distribution<float> l_distribution(0.0, 1.0);
}

void SceneCreator::createSphereDataset(std::vector<SceneCreator::SphereData>& dataset, int numBalls)
{
    SphereData floor{
        {0.0f, -1000.0f, -10.0f},
        1000.0f,
        Material::Type::Lambertian,
        {0.8f, 0.8f, 0.8f},
        0.0f,
        0.0f,
        ""};

    SphereData red{
        {-1.3f, 1.0f, -11.5f},
        1.0f,
        Material::Type::Lambertian,
        {0.8f, 0.3f, 0.3f},
        0.0f,
        0.0f,
        ""};

    SphereData water{
        {1.3f, 1.0f, -6.5f},
        1.0f,
        Material::Type::Refractive,
        {0.0f, 0.0f, 0.0f},
        0.0f,
        1.5f,
        ""};

    SphereData metal{
        {0.0f, 1.0f, -9.0f},
        1.0f,
        Material::Type::Reflective,
        {0.7f, 0.6f, 0.5f},
        0.0f,
        0.0f,
        ""};

    SphereData earth{
        {-2.0f, 1.0f, -6.5f},
        1.0f,
        Material::Type::Lambertian,
        {1.0f, 1.0f, 1.0f},
        0.0f,
        0.0f,
        "../images/earth.jpg"};

    SphereData light1{
        {0.0f, 5.0f, -11.0f},
        1.0f,
        Material::Type::DiffuseLight,
        {5.0f, 3.0f, 3.0f},
        0.0f,
        0.0f,
        ""};

    SphereData light2{
        {3.0f, 5.0f, -7.0f},
        1.0f,
        Material::Type::DiffuseLight,
        {3.0f, 5.0f, 3.0f},
        0.0f,
        0.0f,
        ""};

    SphereData light3{
        {-3.0f, 5.0f, -7.0f},
        1.0f,
        Material::Type::DiffuseLight,
        {3.0f, 3.0f, 5.0f},
        0.0f,
        0.0f,
        ""};

    dataset.push_back(floor);
    dataset.push_back(red);
    dataset.push_back(water);
    dataset.push_back(metal);
    dataset.push_back(earth);
    dataset.push_back(light1);
    dataset.push_back(light2);
    dataset.push_back(light3);

    for (int i = 0; i < numBalls; ++i)
    {
        Material::Type t;
        float p = l_distribution(l_random);
        if (p < 0.4f)
        {
            t = Material::Type::Lambertian;
        }
        else if (p < 0.8f)
        {
            t = Material::Type::Reflective;
        }
        else
        {
            t = Material::Type::Refractive;
        }

        float z = -1.0f - (l_distribution(l_random) * 20.0f); // [-1.0, -20.0]
        float x = (l_distribution(l_random) - 0.5f) * z * tan(Constants::fov);

        SphereData s{
            {x, 0.2f, z},
            0.2f,
            t,
            {l_distribution(l_random), l_distribution(l_random), l_distribution(l_random)},
            l_distribution(l_random),
            l_distribution(l_random) * 2.0f};

        dataset.push_back(s);
    }
}