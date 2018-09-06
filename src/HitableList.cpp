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
