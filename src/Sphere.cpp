#include "Sphere.h"

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
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < max && temp > min)
        {
            hit.t = temp;
            hit.p = ray.pointAt(hit.t);
            hit.normal = (hit.p - m_center) / m_radius;
            hit.material = m_material;
            return true;
        }
    }
    return false;
}
