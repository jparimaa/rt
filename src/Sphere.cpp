#include "Sphere.h"

#include <glm/gtc/constants.hpp>

#include <cmath>

Sphere::Sphere(glm::vec3 center, float radius, Material* material) :
    m_center(center),
    m_radius(radius),
    m_material(material)
{
}

bool Sphere::hit(const Ray& ray, float min, float max, Hit& hit) const
{
    glm::vec3 oc = ray.origin() - m_center;
    float a = glm::dot(ray.direction(), ray.direction());
    float b = glm::dot(oc, ray.direction());
    float c = glm::dot(oc, oc) - m_radius * m_radius;
    float discriminant = b * b - a * c;
    if (discriminant > 0.0f)
    {
        float temp = (-b - sqrt(discriminant)) / a;
        if (temp < max && temp > min)
        {
            hit.t = temp;
            hit.p = ray.pointAt(hit.t);
            hit.normal = (hit.p - m_center) / m_radius;
            hit.material = m_material;
            hit.hitable = this;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < max && temp > min)
        {
            hit.t = temp;
            hit.p = ray.pointAt(hit.t);
            hit.normal = (hit.p - m_center) / m_radius;
            hit.material = m_material;
            hit.hitable = this;
            return true;
        }
    }
    return false;
}

bool Sphere::getBoundingBox(AABB& box)
{
    box = AABB(m_center - glm::vec3(m_radius), m_center + glm::vec3(m_radius));
    return true;
}

glm::vec2 Sphere::getUV(glm::vec3 p) const
{
    p = glm::normalize(p - m_center);
    float phi = atan2(p.z, p.x); // [-pi, pi]
    float theta = asin(p.y); // [-pi/2, pi/2]
    float pi = glm::pi<float>();
    float u = 1.0f - (phi + pi) / (2.0f * pi);
    float v = (theta + pi / 2.0f) / pi;
    return glm::vec2(u, v);
}