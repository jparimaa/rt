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

const int c_width = 400;
const int c_height = 200;
const int c_numSamples = 100;
const int c_maxDepth = 25;
const float c_maxDistance = std::numeric_limits<float>::max();

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

int main()
{
    int percentageInterval = 10;
    int percentageModulo = c_height / percentageInterval;

    std::string fileName = "output.png";

    uint8_t* imageData = (uint8_t*)malloc(c_width * c_height * 3);

    auto startTime = std::chrono::high_resolution_clock::now();
    std::cout << "Samples " << c_numSamples << "\nMax depth " << c_maxDepth << "\n";
    std::cout << "Started running...\n";

    glm::vec3 s = glm::sqrt(glm::vec3(0.49f, 0.25f, 0.9f));

    Lambertian red(glm::vec3(0.8f, 0.3f, 0.3f));
    Lambertian lime(glm::vec3(0.8f, 0.8f, 0.3f));
    Lambertian blue(glm::vec3(0.3f, 0.3f, 1.0f));
    Reflective orange(glm::vec3(0.8f, 0.6f, 0.2f), 0.2f);
    Reflective grey(glm::vec3(0.8f, 0.8f, 0.8f), 0.4f);
    Lambertian pureWhite(glm::vec3(1.0f));
    Refractive water(1.5f);

    HitableList world;
    world.addHitable<Sphere>(glm::vec3(0.0f, 0.0f, -4.0f), 0.5f, &red);
    world.addHitable<Sphere>(glm::vec3(0.0f, -100.5f, -4.0f), 100.0f, &orange);
    world.addHitable<Sphere>(glm::vec3(1.0f, 0.0f, -4.0f), 0.5f, &blue);
    world.addHitable<Sphere>(glm::vec3(0.35f, -0.3f, -3.6f), 0.15f, &water);
    world.addHitable<Sphere>(glm::vec3(-0.35f, -0.3f, -3.6f), 0.15f, &water);
    world.addHitable<Sphere>(glm::vec3(-1.0f, 0.0f, -4.0f), 0.5f, &grey);

    glm::vec3 position(0.0f, 2.0f, 0.0f);
    glm::vec3 lookAt(0.0f, 0.0f, -4.0f);
    glm::vec3 worldUp(0.0f, 1.0f, 0.0f);
    float fov = glm::half_pi<float>() / 2.0f;
    float aspectRatio = c_width / c_height;
    const Camera camera(position, lookAt, worldUp, fov, aspectRatio);

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

        if ((i % percentageModulo) == 0)
        {
            std::cout << "\r" << (float(c_height - i) / float(c_height) * 100.0f) << "%" << std::flush;
        }
    }

    std::cout << "\n";

    auto endTime = std::chrono::high_resolution_clock::now();
    auto time = endTime - startTime;
    std::cout << "Execution time " << std::chrono::duration_cast<std::chrono::seconds>(time).count() << " seconds\n";

    std::cout << "Wrote file " << fileName << "\n";

    stbi_write_png(fileName.c_str(), c_width, c_height, 3, (void*)imageData, 0);

    free((void*)imageData);

    std::cout << "Press Enter to Continue";
    std::cin.ignore();
    return 0;
}