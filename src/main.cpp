#include "Ray.h"
#include "Camera.h"
#include "Hitable.h"
#include "HitableList.h"
#include "Sphere.h"
#include "Common.h"
#include "Material.h"

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

const int c_width = 400;
const int c_height = 200;
const int c_numSamples = 200;
const int c_maxDepth = 100;
const int c_numThreads = 6;
const int c_totalImageSize = c_width * c_height * 3;
const float c_maxDistance = std::numeric_limits<float>::max();

std::unordered_map<std::thread::id, int> g_progress;

std::default_random_engine g_random;
std::uniform_real_distribution<float> g_distribution(0.0, 1.0);

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
            return glm::vec3(0.0f, 0.0f, 0.0f);
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
    Lambertian red(glm::vec3(0.8f, 0.3f, 0.3f));
    Lambertian lime(glm::vec3(0.8f, 0.8f, 0.3f));
    Lambertian blue(glm::vec3(0.3f, 0.3f, 1.0f));
    Reflective orange(glm::vec3(0.8f, 0.6f, 0.2f), 0.2f);
    Reflective grey(glm::vec3(0.8f, 0.8f, 0.8f), 0.4f);
    Reflective pink(glm::vec3(0.8f, 0.2f, 0.8f), 0.4f);
    Lambertian pureWhite(glm::vec3(1.0f));
    Refractive water(1.5f);

    HitableList world;
    world.addHitable<Sphere>(glm::vec3(0.0f, 0.0f, -10.0f), 0.5f, &red);
    world.addHitable<Sphere>(glm::vec3(0.0f, -100.5f, -10.0f), 100.0f, &orange);
    world.addHitable<Sphere>(glm::vec3(0.7f, 0.0f, -7.0f), 0.5f, &blue);
    //world.addHitable<Sphere>(glm::vec3(0.35f, -0.3f, -3.6f), 0.15f, &water);
    //world.addHitable<Sphere>(glm::vec3(-0.35f, -0.3f, -3.6f), 0.15f, &water);
    world.addHitable<Sphere>(glm::vec3(-0.7f, 0.0f, -13.0f), 0.5f, &grey);

    glm::vec3 position(0.0f, 2.0f, 0.0f);
    glm::vec3 lookAt(0.0f, 0.0f, -10.0f);
    glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
    float fov = glm::half_pi<float>() / 2.0f;
    float aspectRatio = c_width / c_height;
    float aperture = 1.0f;
    float focusDistance = glm::distance(lookAt, position);
    const Camera camera(position, lookAt, worldUp, fov, aspectRatio, aperture, focusDistance);

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
                Ray ray = camera.getRay(u, v);

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
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        for (auto& kv : g_progress)
        {
            currentProgress += kv.second;
        }

        float percentage = static_cast<float>(currentProgress) / static_cast<float>(totalProgress);
        std::cout << "\r" << static_cast<int>(percentage * 100.0f) << "%" << std::flush;

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
