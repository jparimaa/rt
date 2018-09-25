#pragma once

#include <glm/glm.hpp>

#include <string>

class Texture
{
public:
    Texture(){};
    ~Texture();
    bool load(const std::string& filename);
    glm::vec3
    getColor(glm::vec2 uv);

private:
    std::string m_filename;
    int m_width = -1;
    int m_height = -1;
    float m_fWidth = -1.0f;
    float m_fHeight = -1.0f;
    int m_channels = -1;
    uint8_t* m_data = nullptr;
};