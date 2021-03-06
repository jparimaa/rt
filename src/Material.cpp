#include "Material.h"
#include "Common.h"

glm::vec3 Material::emit() const
{
    return glm::vec3(0.0f);
}

Lambertian::Lambertian(glm::vec3 albedo, Texture* texture) :
    m_albedo(albedo),
    m_texture(texture)
{
}

bool Lambertian::scatter(const Ray& /*incoming*/, const Hit& hit, glm::vec3& attenuation, Ray& outgoing) const
{
    glm::vec3 target = hit.p + hit.normal + randomInUnitSphere();
    outgoing = Ray(hit.p, target - hit.p);
    attenuation = m_albedo;
    if (m_texture != nullptr)
    {
        glm::vec2 uv = hit.hitable->getUV(hit.p);
        attenuation *= m_texture->getColor(uv);
    }
    return true;
}

Reflective::Reflective(glm::vec3 albedo, float fuzziness) :
    m_albedo(albedo),
    m_fuzziness(fuzziness)
{
}

bool Reflective::scatter(const Ray& incoming, const Hit& hit, glm::vec3& attenuation, Ray& outgoing) const
{
    glm::vec3 reflected = glm::reflect(glm::normalize(incoming.direction()), hit.normal);
    outgoing = Ray(hit.p, reflected + m_fuzziness * randomInUnitSphere());
    attenuation = m_albedo;
    return glm::dot(outgoing.direction(), hit.normal) > (0.0f - std::numeric_limits<float>::epsilon());
}

Refractive::Refractive(float refractionIndex) :
    m_refractionIndex(refractionIndex)
{
}

bool Refractive::scatter(const Ray& incoming, const Hit& hit, glm::vec3& attenuation, Ray& outgoing) const
{
    // Transparent objects do not cause attenuation
    attenuation = glm::vec3(1.0, 1.0, 1.0);

    const float airRefractionIndex = 1.0f;

    glm::vec3 outwardNormal;
    float refractionRatio;
    if (glm::dot(incoming.direction(), hit.normal) > 0.0f) // From material to air
    {
        outwardNormal = -hit.normal;
        refractionRatio = m_refractionIndex / airRefractionIndex;
    }
    else // From air to material
    {
        outwardNormal = hit.normal;
        refractionRatio = airRefractionIndex / m_refractionIndex;
    }

    auto refract = [](const glm::vec3& incoming, const glm::vec3& n, float refractionRatio, glm::vec3& outgoing) {
        // https: //en.wikipedia.org/wiki/Snell%27s_law#Vector_form
        glm::vec3 l = glm::normalize(incoming);
        float r = refractionRatio;
        float c = glm::dot(l, n);
        float discriminant = 1.0f - r * r * (1.0f - c * c);
        if (discriminant > 0.0f)
        {
            outgoing = (r * l) - ((r * c) + static_cast<float>(sqrt(discriminant))) * n;
            return true;
        }
        return false;
    };

    glm::vec3 refracted;
    if (refract(incoming.direction(), outwardNormal, refractionRatio, refracted))
    {
        outgoing = Ray(hit.p, refracted);
    }
    else
    {
        glm::vec3 reflected = glm::reflect(glm::normalize(incoming.direction()), hit.normal);
        outgoing = Ray(hit.p, reflected);
    }
    return true;
}

DiffuseLight::DiffuseLight(glm::vec3 color) :
    m_color(color)
{
}

bool DiffuseLight::scatter(const Ray& incoming, const Hit& hit, glm::vec3& attenuation, Ray& outgoing) const
{
    return false;
}

glm::vec3 DiffuseLight::emit() const
{
    return m_color;
}