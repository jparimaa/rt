#pragma once

#include "Ray.h"
#include "Hitable.h"

#include <glm/glm.hpp>

class Material
{
public:
    virtual bool scatter(const Ray& incoming, const Hit& hit, glm::vec3& attenuation, Ray& outgoing) const = 0;
};

class Lambertian : public Material
{
public:
    Lambertian(glm::vec3 albedo);
    virtual bool scatter(const Ray& incoming, const Hit& hit, glm::vec3& attenuation, Ray& outgoing) const;

private:
    glm::vec3 m_albedo;
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