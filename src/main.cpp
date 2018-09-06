#include "Ray.h"
#include "Camera.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Sphere.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

#include <string>
#include <cmath>
#include <iostream>
#include <random>
#include <chrono>

const int c_width = 400;
const int c_height = 200;
const int c_numSamples = 50;
const float c_maxDistance = std::numeric_limits<float>::max();

const Camera g_camera;
std::default_random_engine g_random;
std::uniform_real_distribution<float> g_distribution(0.0, 1.0);
int g_maxDepth = 0;

glm::vec3 randomInUnitSphere()
{
    glm::vec3 p;
    do
    {
        // 2.0f * x - 1.0f to make it between [-1..1)
        p = 2.0f * glm::vec3(g_distribution(g_random), g_distribution(g_random), g_distribution(g_random)) - glm::vec3(1.0f);
    } while (glm::length2(p) >= 1.0f); // Squared length is faster and does the same thing in this situation
    return p;
}

glm::vec3 calculateColor(const Ray& ray, const Hitable& world, int& depth)
{
    ++depth;
    g_maxDepth = depth > g_maxDepth ? depth : g_maxDepth;
    HitRecord rec;
    if (world.hit(ray, 0.001f, c_maxDistance, rec))
    {
        glm::vec3 target = rec.p + rec.normal + randomInUnitSphere();
        return 0.5f * calculateColor(Ray(rec.p, target - rec.p), world, depth);
    }
    else
    {
        float t = 0.5f * (glm::normalize(ray.direction()).y + 1.0f);
        return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
    }
}

glm::vec3 visualizeNormals(const Ray& ray, const Hitable& world)
{
    HitRecord record;
    if (world.hit(ray, 0.0f, c_maxDistance, record))
    {
        // Clamp between [0..1] since unsigned integers don't like to go negative
        return 0.5f * (glm::vec3(record.normal.x, record.normal.y, record.normal.z) + glm::vec3(1.0f));
    }
    else
    {
        float t = 0.5f * (glm::normalize(ray.direction()).y + 1.0f);
        return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
    }
}

int main()
{
    int percentageInterval = 10;
    int percentageModulo = c_height / percentageInterval;

    std::string fileName = "output.png";

    uint8_t* imageData = (uint8_t*)malloc(c_width * c_height * 3);

    auto startTime = std::chrono::high_resolution_clock::now();
    std::cout << "Samples " << c_numSamples << "\n";
    std::cout << "Started running...\n";

    glm::vec3 s = glm::sqrt(glm::vec3(0.49f, 0.25f, 0.9f));

    HitableList world;
    world.addHitable<Sphere>(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f);
    world.addHitable<Sphere>(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f);

    int counter = 0;
    for (int i = c_height - 1; i >= 0; --i)
    {
        for (int j = 0; j < c_width; ++j)
        {
            glm::vec3 output(0.0f);
            for (int k = 0; k < c_numSamples; ++k)
            {
                float u = float(j + g_distribution(g_random)) / float(c_width);
                float v = float(i + g_distribution(g_random)) / float(c_height);
                Ray ray = g_camera.getRay(u, v);

                int depth = 0;

                output += calculateColor(ray, world, depth);
                //output += visualizeNormals(ray, world);
            }

            output = output / float(c_numSamples);
            output = glm::sqrt(output) * 255.9f;

            imageData[counter++] = uint8_t(output.r);
            imageData[counter++] = uint8_t(output.g);
            imageData[counter++] = uint8_t(output.b);
        }

        if ((i % percentageModulo) == 0)
        {
            std::cout << (float(c_height - i) / float(c_height) * 100.0f) << "% ";
        }
    }

    std::cout << "\n";

    auto endTime = std::chrono::high_resolution_clock::now();
    auto time = endTime - startTime;
    std::cout << "Execution time " << std::chrono::duration_cast<std::chrono::milliseconds>(time).count() << "ms\n";

    std::cout << "Wrote file " << fileName << "\n";

    stbi_write_png(fileName.c_str(), c_width, c_height, 3, (void*)imageData, 0);

    free((void*)imageData);

    std::cout << "Max depth " << g_maxDepth << "\n";

    std::cout << "Press Enter to Continue";
    std::cin.ignore();
    return 0;
}