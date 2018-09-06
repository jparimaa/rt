#include "Sphere.h"

#include <cmath>

Sphere::Sphere(glm::vec3 center, float radius) :
    m_center(center),
    m_radius(radius)
{
}

bool Sphere::hit(const Ray& ray, float min, float max, HitRecord& record) const
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
            record.t = temp;
            record.p = ray.pointAt(record.t);
            record.normal = (record.p - m_center) / m_radius;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < max && temp > min)
        {
            record.t = temp;
            record.p = ray.pointAt(record.t);
            record.normal = (record.p - m_center) / m_radius;
            return true;
        }
    }
    return false;
}
