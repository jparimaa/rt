#include "Ray.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <glm/glm.hpp>

#include <string>
#include <iostream>

bool hitSphere(const glm::vec3& center, float radius, const Ray& r)
{
    glm::vec3 oc = r.origin() - center;
    float a = glm::dot(r.direction(), r.direction());
    float b = 2.0f * glm::dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;
    return discriminant > 0.0f;
}

int main()
{
    int width = 200;
    int height = 100;

    std::string fileName = "output.png";

    uint8_t* imageData = (uint8_t*)malloc(width * height * 3);

    glm::vec3 lowerLeftCorner(-2.0f, -1.0f, -1.0f);
    glm::vec3 horizontal(4.0f, 0.0f, 0.0f);
    glm::vec3 vertical(0.0f, 2.0f, 0.0f);
    glm::vec3 origin(0.0f, 0.0f, 0.0f);

    int counter = 0;
    for (int i = height - 1; i >= 0; --i)
    {
        for (int j = 0; j < width; ++j)
        {
            float u = float(j) / float(width);
            float v = float(i) / float(height);

            glm::vec3 direction(lowerLeftCorner + u * horizontal + v * vertical);
            Ray r(origin, direction);

            glm::vec3 output;

            if (hitSphere({0.0f, 0.0f, -1.0f}, 0.5f, r))
            {
                output = {255.0f, 0.0f, 0.0f};
            }
            else
            {
                output = {255.9 * u, 255.9 * v, 255.9 * 0.2f};
            }

            imageData[counter++] = uint8_t(output.r);
            imageData[counter++] = uint8_t(output.g);
            imageData[counter++] = uint8_t(output.b);
        }
    }

    std::cout << "Wrote file " << fileName << "\n";

    stbi_write_png(fileName.c_str(), width, height, 3, (void*)imageData, 0);

    free((void*)imageData);

    std::cout << "Press Enter to Continue";
    std::cin.ignore();
    return 0;
}