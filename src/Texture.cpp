#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <algorithm>
#include <iostream>

Texture::~Texture()
{
    stbi_image_free(m_data);
}

bool Texture::load(const std::string& filename)
{
    m_filename = filename;
    stbi_set_flip_vertically_on_load(true);
    m_data = stbi_load(filename.c_str(), &m_width, &m_height, &m_channels, 0);
    if (m_data == nullptr)
    {
        std::cerr << "ERROR: Unable to read file " << filename << "\n";
        return false;
    }
    m_fWidth = static_cast<float>(m_width);
    m_fHeight = static_cast<float>(m_height);
    return true;
}

glm::vec3 Texture::getColor(glm::vec2 uv)
{
    if (m_data == nullptr)
    {
        std::cerr << "ERROR: Trying to read from an unloaded texture\n";
        return glm::vec3(0.0f);
    }
    int x = static_cast<int>(uv.x * m_fWidth);
    int y = static_cast<int>(uv.y * m_fHeight);
    x = std::min(m_width, std::max(0, x));
    y = std::min(m_height, std::max(0, y));
    int rows = 3 * y * m_width;
    int column = 3 * x;
    float r = static_cast<float>(m_data[rows + column + 0]) / 255.0f;
    float g = static_cast<float>(m_data[rows + column + 1]) / 255.0f;
    float b = static_cast<float>(m_data[rows + column + 2]) / 255.0f;
    return glm::vec3(r, g, b);
}