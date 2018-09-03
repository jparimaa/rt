#include "Ray.h"
#include "Camera.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <glm/glm.hpp>

#include <string>
#include <cmath>
#include <iostream>
#include <random>
#include <chrono>

float hitSphere(const glm::vec3& center, float radius, const Ray& r)
{
    glm::vec3 oc = r.origin() - center;
    float a = glm::dot(r.direction(), r.direction());
    float b = 2.0f * glm::dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0.0f)
    {
        return -1.0f;
    }

    return (-b - sqrt(discriminant)) / 2.0f * a;
}

int main()
{
    int width = 400;
    int height = 200;
    int numSamples = 20;
    int percentageInterval = 10;
    int percentageModulo = height / percentageInterval;

    std::string fileName = "output.png";

    uint8_t* imageData = (uint8_t*)malloc(width * height * 3);

    Camera camera;
    std::default_random_engine generator;
    std::uniform_real_distribution<float> distribution(0.0, 1.0);
    glm::vec3 sphereCenter(0.0f, 0.0f, -1.0f);

    auto startTime = std::chrono::high_resolution_clock::now();
    std::cout << "Started running...\n";

    int counter = 0;
    for (int i = height - 1; i >= 0; --i)
    {
        for (int j = 0; j < width; ++j)
        {
            glm::vec3 output(0.0f);

            for (int k = 0; k < numSamples; ++k)
            {
                float u = float(j + distribution(generator)) / float(width);
                float v = float(i + distribution(generator)) / float(height);
                Ray ray = camera.getRay(u, v);

                float t = hitSphere(sphereCenter, 0.5f, ray);
                if (t > 0.0f)
                {
                    glm::vec3 N = glm::normalize(ray.pointAt(t) - sphereCenter);
                    N = (N + 1.0f) * 0.5f; // Negative numbers give artifacts on uint so map top [0..1]
                    output += N;
                }
            }

            output = output / float(numSamples) * 255.9f;

            imageData[counter++] = uint8_t(output.r);
            imageData[counter++] = uint8_t(output.g);
            imageData[counter++] = uint8_t(output.b);
        }

        if ((i % percentageModulo) == 0)
        {
            std::cout << (float(height - i) / float(height) * 100.0f) << "% ";
        }
    }

    std::cout << "\n";

    auto endTime = std::chrono::high_resolution_clock::now();
    auto time = endTime - startTime;
    std::cout << "Execution time " << std::chrono::duration_cast<std::chrono::milliseconds>(time).count() << "ms\n";

    std::cout << "Wrote file " << fileName << "\n";

    stbi_write_png(fileName.c_str(), width, height, 3, (void*)imageData, 0);

    free((void*)imageData);

    std::cout << "Press Enter to Continue";
    std::cin.ignore();
    return 0;
}