#pragma once

#include "Hitable.h"
#include "AABB.h"
#include "Ray.h"

#include <glm/glm.hpp>

#include <vector>

class BVHNode : public Hitable
{
public:
    BVHNode(std::vector<Hitable*> hitables);
    virtual ~BVHNode();

    virtual bool hit(const Ray& ray, float min, float max, Hit& hit) const;
    virtual bool getBoundingBox(AABB& box);
    virtual glm::vec2 getUV(glm::vec3 p) const;

private:
    AABB m_box;
    Hitable* m_left;
    Hitable* m_right;
};
