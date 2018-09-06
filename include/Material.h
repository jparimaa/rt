#pragma once

#include "Ray.h"
#include "Hitable.h"

#include <glm/glm.hpp>

class Material
{
public:
    virtual bool scatter(const Ray& incoming, const Hit& hit, glm::vec3& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material
{
public:
    Lambertian(glm::vec3 albedo);
    virtual bool scatter(const Ray& incoming, const Hit& hit, glm::vec3& attenuation, Ray& scattered) const;

private:
    glm::vec3 m_albedo;
};

class Metal : public Material
{
public:
    Metal(glm::vec3 albedo);
    virtual bool scatter(const Ray& incoming, const Hit& hit, glm::vec3& attenuation, Ray& scattered) const;

private:
    glm::vec3 m_albedo;
};