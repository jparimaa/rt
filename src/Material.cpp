#include "Material.h"
#include "Common.h"

Lambertian::Lambertian(glm::vec3 albedo) :
    m_albedo(albedo)
{
}

bool Lambertian::scatter(const Ray& /*incoming*/, const Hit& hit, glm::vec3& attenuation, Ray& scattered) const
{
    glm::vec3 target = hit.p + hit.normal + randomInUnitSphere();
    scattered = Ray(hit.p, target - hit.p);
    attenuation = m_albedo;
    return true;
}

Metal::Metal(glm::vec3 albedo, float fuzziness) :
    m_albedo(albedo),
    m_fuzziness(fuzziness)
{
}

bool Metal::scatter(const Ray& incoming, const Hit& hit, glm::vec3& attenuation, Ray& scattered) const
{
    glm::vec3 reflected = glm::reflect(glm::normalize(incoming.direction()), hit.normal);
    scattered = Ray(hit.p, reflected + m_fuzziness * randomInUnitSphere());
    attenuation = m_albedo;
    return glm::dot(scattered.direction(), hit.normal) > 0.0f;
}