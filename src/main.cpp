#include "Ray.h"
#include "Camera.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Common.h"
#include "Material.h"
#include "BVHNode.h"
#include "Texture.h"
#include "SceneCreator.h"
#include "Constants.h"
#include "Stopwatch.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <glm/glm.hpp>

#include <string>
#include <cmath>
#include <iostream>
#include <chrono>
#include <random>
#include <thread>
#include <unordered_map>
#include <utility>
#include <memory>
#include <atomic>

std::atomic<int> g_threadsRunning = 0;
std::unordered_map<std::thread::id, int> g_progress;

std::default_random_engine g_random;
std::uniform_real_distribution<float> g_distribution(0.0, 1.0);
std::vector<SceneCreator::SphereData> g_sphereDataset;

glm::vec3 calculateColor(const Ray& ray, const Hitable& world, int depth)
{
    Hit hit;
    if (world.hit(ray, 0.001f, Constants::maxDistance, hit))
    {
        glm::vec3 attenuation;
        Ray scattered;
        glm::vec3 emitted = hit.material->emit();
        if (depth < Constants::maxDepth && hit.material->scatter(ray, hit, attenuation, scattered))
        {
            return emitted + attenuation * calculateColor(scattered, world, ++depth);
        }
        else
        {
            return emitted;
        }
    }
    else
    {
        return glm::vec3(0.1f);
    }
}

glm::vec3 visualizeNormals(const Ray& ray, const Hitable& world)
{
    Hit hit;
    if (world.hit(ray, 0.0f, Constants::maxDistance, hit))
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
    ++g_threadsRunning;
    std::vector<std::unique_ptr<Material>> materials;
    std::vector<std::unique_ptr<Sphere>> spheres;
    std::vector<Texture> textures;
    std::vector<Hitable*> hitables;
    for (const SceneCreator::SphereData& data : g_sphereDataset)
    {
        std::unique_ptr<Material> material;
        if (data.type == Material::Type::Lambertian)
        {
            if (!data.texture.empty())
            {
                textures.resize(textures.size() + 1);
                textures.back().load(data.texture);
                material.reset(new Lambertian(data.color, &textures.back()));
            }
            else
            {
                material.reset(new Lambertian(data.color));
            }
        }
        else if (data.type == Material::Type::Reflective)
        {
            material.reset(new Reflective(data.color, data.fuzziness));
        }
        else if (data.type == Material::Type::Refractive)
        {
            material.reset(new Refractive(data.refractionIndex));
        }
        else if (data.type == Material::Type::DiffuseLight)
        {
            material.reset(new DiffuseLight(data.color));
        }
        else
        {
            std::cerr << "ERROR: Invalid material type\n";
            return;
        }
        materials.push_back(std::move(material));
        std::unique_ptr<Sphere> sphere(new Sphere(data.position, data.radius, materials.back().get()));
        spheres.push_back(std::move(sphere));
        hitables.push_back(spheres.back().get());
    }

    BVHNode world(hitables);

    int counter = start * Constants::width * 3;
    int startHeight = Constants::height - 1 - start;
    int endHeight = Constants::height - end;
    int progressCounter = 0;

    for (int i = startHeight; i >= endHeight; --i)
    {
        ++g_progress[std::this_thread::get_id()];
        for (int j = 0; j < Constants::width; ++j)
        {
            glm::vec3 output(0.0f);
            for (int k = 0; k < Constants::numSamples; ++k)
            {
                float u = float(j + g_distribution(g_random)) / float(Constants::width);
                float v = float(i + g_distribution(g_random)) / float(Constants::height);
                Ray ray = Constants::camera.getRay(u, v);

                output += calculateColor(ray, world, 0);
                //output += visualizeNormals(ray, world);
            }

            output = output / float(Constants::numSamples);
            output = glm::clamp(glm::sqrt(output) * 255.9f, glm::vec3(0.0f), glm::vec3(255.0f));

            imageData[counter++] = uint8_t(output.r);
            imageData[counter++] = uint8_t(output.g);
            imageData[counter++] = uint8_t(output.b);
        }
    }
    --g_threadsRunning;
}

int main()
{
    SceneCreator::createSphereDataset(g_sphereDataset, 10);

    std::cout << "Total image size " << (Constants::totalImageSize / 1000) << " kb\n"
              << "Threads " << Constants::numThreads << "\n"
              << "Samples " << Constants::numSamples << "\n"
              << "Max depth " << Constants::maxDepth << "\n"
              << "Started running...\n";

    Stopwatch stopwatch;
    stopwatch.start();

    uint8_t* imageData = (uint8_t*)malloc(Constants::totalImageSize);

    int remainder = Constants::height % Constants::numThreads;
    int heightPerThread = Constants::height / Constants::numThreads;
    std::vector<std::thread> threads;
    for (int i = 0; i < Constants::numThreads; ++i)
    {
        int start = i * heightPerThread;
        int end = (i + 1) * heightPerThread;
        end = i == (Constants::numThreads - 1) ? end + remainder : end;
        threads.push_back(std::thread(executeSection, start, end, imageData));
    }

    const int totalProgress = Constants::height;
    int currentProgress = 0;
    while (true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        for (auto& kv : g_progress)
        {
            currentProgress += kv.second;
        }

        float percentage = static_cast<float>(currentProgress) / static_cast<float>(totalProgress);
        float secondsPassed = static_cast<float>(stopwatch.getTimeSinceStart());
        float eta = (1.0f / percentage) * secondsPassed;
        eta -= secondsPassed;
        eta *= log2f(static_cast<float>(g_threadsRunning + 1));

        std::cout << "\r" << static_cast<int>(percentage * 100.0f) << "% completed, elapsed " << secondsPassed << "s, remaining " << static_cast<int>(eta) << "s, threads " << g_threadsRunning << "   " << std::flush;

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

    std::cout << "Execution time " << stopwatch.getTimeSinceStart() << " seconds\n";

    std::string fileName = "output.png";
    stbi_write_png(fileName.c_str(), Constants::width, Constants::height, 3, (void*)imageData, 0);
    std::cout << "Wrote file " << fileName << "\n";

    free((void*)imageData);

    std::cout << "Press Enter to Continue";
    std::cin.ignore();
    return 0;
}
