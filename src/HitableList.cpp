#include "HitableList.h"

HitableList::~HitableList()
{
    for (Hitable* hitable : m_list)
    {
        delete hitable;
    }
}

bool HitableList::hit(const Ray& r, float min, float max, HitRecord& record) const
{
    HitRecord tempRecord;
    bool hitAnything = false;
    float closestSoFar = max;
    for (Hitable* hitable : m_list)
    {
        if (hitable->hit(r, min, closestSoFar, tempRecord))
        {
            hitAnything = true;
            closestSoFar = tempRecord.t;
            record = tempRecord;
        }
    }
    return hitAnything;
}
