#include "Ray.h"
#include "Camera.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Common.h"
#include "Material.h"
#include "BVHNode.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include <string>
#include <cmath>
#include <iostream>
#include <chrono>
#include <random>
#include <thread>
#include <unordered_map>
#include <utility>
#include <memory>

enum class MaterialType
{
    Lambertian,
    Refractive,
    Reflective,
};

struct SphereData
{
    glm::vec3 position;
    float radius;
    MaterialType type;
    glm::vec3 color;
    float fuzziness;
    float refractionIndex;
};

const int c_width = 640;
const int c_height = 480;
const int c_numSamples = 150;
const int c_maxDepth = 10;
const int c_numThreads = 4;
const int c_totalImageSize = c_width * c_height * 3;
const float c_maxDistance = std::numeric_limits<float>::max();
const glm::vec3 c_position(0.0f, 2.5f, 0.0f);
const glm::vec3 c_lookAt(0.0f, 0.0f, -10.0f);
const glm::vec3 c_worldUp(0.0f, 1.0f, 0.0f);
const float c_fov = glm::half_pi<float>() / 2.0f;
const float c_aspectRatio = static_cast<float>(c_width) / static_cast<float>(c_height);
const float c_aperture = 0.10f;
const float c_focusDistance = glm::distance(c_lookAt, c_position);
const Camera c_camera(c_position, c_lookAt, c_worldUp, c_fov, c_aspectRatio, c_aperture, c_focusDistance);
const int c_numBalls = 300;

std::unordered_map<std::thread::id, int> g_progress;

std::default_random_engine g_random;
std::uniform_real_distribution<float> g_distribution(0.0, 1.0);
std::vector<SphereData> g_sphereDataset;

void createSphereDataset()
{
    SphereData floor{
        {0.0f, -1000.0f, -10.0f},
        1000.0f,
        MaterialType::Lambertian,
        {0.8f, 0.8f, 0.8f},
        0.0f,
        0.0f};

    SphereData red{
        {-1.3f, 1.0f, -11.5f},
        1.0f,
        MaterialType::Lambertian,
        {0.8f, 0.3f, 0.3f},
        0.0f,
        0.0f};

    SphereData water{
        {0.0f, 1.0f, -9.0f},
        1.0f,
        MaterialType::Refractive,
        {0.0f, 0.0f, 0.0f},
        0.0f,
        1.5f};

    SphereData metal{
        {1.3f, 1.0f, -6.5f},
        1.0f,
        MaterialType::Reflective,
        {0.7f, 0.6f, 0.5f},
        0.15f,
        0.0f};

    g_sphereDataset.push_back(floor);
    g_sphereDataset.push_back(red);
    g_sphereDataset.push_back(water);
    g_sphereDataset.push_back(metal);

    for (int i = 0; i < c_numBalls; ++i)
    {
        MaterialType t;
        float p = g_distribution(g_random);
        if (p < 0.4f)
        {
            t = MaterialType::Lambertian;
        }
        else if (p < 0.8f)
        {
            t = MaterialType::Reflective;
        }
        else
        {
            t = MaterialType::Refractive;
        }

        float z = -1.0f - (g_distribution(g_random) * 20.0f);
        float x = (g_distribution(g_random) - 0.5f) * z * tan(c_fov);

        SphereData s{
            {x, 0.2f, z},
            0.2f,
            t,
            {g_distribution(g_random), g_distribution(g_random), g_distribution(g_random)},
            g_distribution(g_random),
            g_distribution(g_random) * 2.0f};

        g_sphereDataset.push_back(s);
    }
}

glm::vec3 calculateColor(const Ray& ray, const Hitable& world, int depth)
{
    Hit hit;
    if (world.hit(ray, 0.001f, c_maxDistance, hit))
    {
        glm::vec3 attenuation;
        Ray scattered;
        if (depth < c_maxDepth && hit.material->scatter(ray, hit, attenuation, scattered))
        {
            return attenuation * calculateColor(scattered, world, ++depth);
        }
        else
        {
            return glm::vec3(1.0f, 0.0f, 0.0f);
        }
    }
    else
    {
        float t = 0.5f * (glm::normalize(ray.direction()).y + 1.0f);
        return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
    }
}

glm::vec3 visualizeNormals(const Ray& ray, const Hitable& world)
{
    Hit hit;
    if (world.hit(ray, 0.0f, c_maxDistance, hit))
    {
        // Clamp between [0..1] since unsigned integers don't like to go negative
        return 0.5f * (glm::vec3(hit.normal.x, hit.normal.y, hit.normal.z) + glm::vec3(1.0f));
    }
    else
    {
        float t = 0.5f * (glm::normalize(ray.direction()).y + 1.0f);
        return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(0.5f, 0.7f, 1.0f);
    }
}

void executeSection(int start, int end, uint8_t* imageData)
{
    std::vector<std::unique_ptr<Material>> materials;
    std::vector<std::unique_ptr<Sphere>> spheres;
    std::vector<Hitable*> hitables;
    for (const SphereData& data : g_sphereDataset)
    {
        std::unique_ptr<Material> material;
        if (data.type == MaterialType::Lambertian)
        {
            material.reset(new Lambertian(data.color));
        }
        else if (data.type == MaterialType::Reflective)
        {
            material.reset(new Reflective(data.color, data.fuzziness));
        }
        else
        {
            material.reset(new Refractive(data.refractionIndex));
        }
        materials.push_back(std::move(material));
        std::unique_ptr<Sphere> sphere(new Sphere(data.position, data.radius, materials.back().get()));
        spheres.push_back(std::move(sphere));
        hitables.push_back(spheres.back().get());
    }

    BVHNode world(hitables);

    int counter = start * c_width * 3;
    int startHeight = c_height - 1 - start;
    int endHeight = c_height - end;
    int progressCounter = 0;

    for (int i = startHeight; i >= endHeight; --i)
    {
        ++g_progress[std::this_thread::get_id()];
        for (int j = 0; j < c_width; ++j)
        {
            glm::vec3 output(0.0f);
            for (int k = 0; k < c_numSamples; ++k)
            {
                float u = float(j + g_distribution(g_random)) / float(c_width);
                float v = float(i + g_distribution(g_random)) / float(c_height);
                Ray ray = c_camera.getRay(u, v);

                output += calculateColor(ray, world, 0);
                //output += visualizeNormals(ray, world);
            }

            output = output / float(c_numSamples);
            output = glm::sqrt(output) * 255.9f;

            imageData[counter++] = uint8_t(output.r);
            imageData[counter++] = uint8_t(output.g);
            imageData[counter++] = uint8_t(output.b);
        }
    }
}

int main()
{
    createSphereDataset();

    std::cout << "Total image size " << (c_totalImageSize / 1000) << " kb\n"
              << "Threads " << c_numThreads << "\n"
              << "Samples " << c_numSamples << "\n"
              << "Max depth " << c_maxDepth << "\n"
              << "Started running...\n";

    auto startTime = std::chrono::high_resolution_clock::now();
    uint8_t* imageData = (uint8_t*)malloc(c_totalImageSize);

    int remainder = c_height % c_numThreads;
    int heightPerThread = c_height / c_numThreads;
    std::vector<std::thread> threads;
    for (int i = 0; i < c_numThreads; ++i)
    {
        int start = i * heightPerThread;
        int end = (i + 1) * heightPerThread;
        end = i == (c_numThreads - 1) ? end + remainder : end;
        threads.push_back(std::thread(executeSection, start, end, imageData));
    }

    const int totalProgress = c_height;
    int currentProgress = 0;
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        for (auto& kv : g_progress)
        {
            currentProgress += kv.second;
        }

        float percentage = static_cast<float>(currentProgress) / static_cast<float>(totalProgress);
        auto currentTime = std::chrono::high_resolution_clock::now();
        auto time = currentTime - startTime;
        float secondsPassed = static_cast<float>(std::chrono::duration_cast<std::chrono::seconds>(time).count());
        int eta = static_cast<int>(((1.0f / percentage) * secondsPassed) - secondsPassed);

        std::cout << "\r" << static_cast<int>(percentage * 100.0f) << "% completed, approximately " << eta << " seconds left " << std::flush;

        if (currentProgress == totalProgress)
        {
            break;
        }

        currentProgress = 0;
    }
    std::cout << "\n";

    for (std::thread& t : threads)
    {
        t.join();
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto time = endTime - startTime;
    std::cout << "Execution time " << std::chrono::duration_cast<std::chrono::seconds>(time).count() << " seconds\n";

    std::string fileName = "output.png";
    stbi_write_png(fileName.c_str(), c_width, c_height, 3, (void*)imageData, 0);
    std::cout << "Wrote file " << fileName << "\n";

    free((void*)imageData);

    std::cout << "Press Enter to Continue";
    std::cin.ignore();
    return 0;
}
