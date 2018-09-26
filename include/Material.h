#pragma once

#include "Ray.h"
#include "Hitable.h"
#include "Texture.h"

#include <glm/glm.hpp>

class Material
{
public:
    virtual bool scatter(const Ray& incoming, const Hit& hit, glm::vec3& attenuation, Ray& outgoing) const = 0;
    virtual glm::vec3 emit() const;
};

class Lambertian : public Material
{
public:
    Lambertian(glm::vec3 albedo, Texture* texture = nullptr);
    virtual bool scatter(const Ray& incoming, const Hit& hit, glm::vec3& attenuation, Ray& outgoing) const;

private:
    glm::vec3 m_albedo;
    Texture* m_texture = nullptr;
};

class Reflective : public Material
{
public:
    Reflective(glm::vec3 albedo, float fuzziness);
    virtual bool scatter(const Ray& incoming, const Hit& hit, glm::vec3& attenuation, Ray& outgoing) const;

private:
    glm::vec3 m_albedo;
    float m_fuzziness = 0.0f;
};

class Refractive : public Material
{
public:
    Refractive(float refractionIndex);
    virtual bool scatter(const Ray& incoming, const Hit& hit, glm::vec3& attenuation, Ray& outgoing) const;

private:
    float m_refractionIndex;
};

class DiffuseLight : public Material
{
public:
    DiffuseLight(glm::vec3 color);
    virtual bool scatter(const Ray& incoming, const Hit& hit, glm::vec3& attenuation, Ray& outgoing) const;
    virtual glm::vec3 emit() const override;

private:
    glm::vec3 m_color;
};