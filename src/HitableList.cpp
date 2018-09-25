#include "HitableList.h"

HitableList::~HitableList()
{
    for (Hitable* hitable : m_list)
    {
        delete hitable;
    }
}

bool HitableList::hit(const Ray& r, float min, float max, Hit& hit) const
{
    Hit tempHit;
    bool hitAnything = false;
    float closestSoFar = max;
    for (Hitable* hitable : m_list)
    {
        if (hitable->hit(r, min, closestSoFar, tempHit))
        {
            hitAnything = true;
            closestSoFar = tempHit.t;
            hit = tempHit;
        }
    }
    return hitAnything;
}

bool HitableList::getBoundingBox(AABB& box)
{
    if (m_list.empty())
    {
        return false;
    }
    AABB tempBox;
    bool validFirstBox = m_list[0]->getBoundingBox(tempBox);
    if (!validFirstBox)
    {
        return false;
    }

    box = tempBox;

    for (Hitable* hitable : m_list)
    {
        if (hitable->getBoundingBox(tempBox))
        {
            box = AABB(box, tempBox);
        }
        else
        {
            return false;
        }
    }
    return true;
}

glm::vec2 HitableList::getUV(glm::vec3 p) const
{
    return glm::vec2(0.0f);
}