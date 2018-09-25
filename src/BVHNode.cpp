#include "BVHNode.h"

#include <algorithm>
#include <iostream>

BVHNode::BVHNode(std::vector<Hitable*> hitables)
{
    if (hitables.size() == 1)
    {
        m_left = hitables[0];
        m_right = hitables[0];
    }
    else if (hitables.size() == 2)
    {
        m_left = hitables[0];
        m_right = hitables[1];
    }
    else
    {
        int axis = rand() % 2;
        std::sort(hitables.begin(), hitables.end(), [axis](Hitable* a, Hitable* b) {
            AABB aBox;
            AABB bBox;
            a->getBoundingBox(aBox);
            b->getBoundingBox(bBox);
            return aBox.getMin()[axis] < bBox.getMin()[axis];
        });

        std::size_t const halfSize = hitables.size() / 2;
        std::vector<Hitable*> firstHalf(hitables.begin(), hitables.begin() + halfSize);
        std::vector<Hitable*> secondHalf(hitables.begin() + halfSize, hitables.end());
        m_left = new BVHNode(firstHalf);
        m_right = new BVHNode(secondHalf);
    }
    AABB boxLeft;
    AABB boxRight;
    m_left->getBoundingBox(boxLeft);
    m_right->getBoundingBox(boxRight);
    m_box = AABB(boxLeft, boxRight);
}

BVHNode::~BVHNode()
{
    auto deleteNode = [](Hitable* node) {
        if (node == nullptr)
        {
            return;
        }
        BVHNode* temp = dynamic_cast<BVHNode*>(node);
        if (temp)
        {
            delete temp;
        }
    };

    deleteNode(m_left);
    deleteNode(m_right);
}

bool BVHNode::hit(const Ray& ray, float min, float max, Hit& hit) const
{
    if (m_box.hit(ray, min, max))
    {
        Hit leftHit;
        Hit rightHit;
        bool isLeftHit = m_left->hit(ray, min, max, leftHit);
        bool isRightHit = m_right->hit(ray, min, max, rightHit);
        if (isLeftHit && isRightHit)
        {
            hit = leftHit.t < rightHit.t ? leftHit : rightHit;
            return true;
        }
        else if (isLeftHit)
        {
            hit = leftHit;
            return true;
        }
        else if (isRightHit)
        {
            hit = rightHit;
            return true;
        }
    }
    return false;
}

bool BVHNode::getBoundingBox(AABB& box)
{
    box = m_box;
    return true;
}

glm::vec2 BVHNode::getUV(glm::vec3 p) const
{
    return glm::vec2(0.0f);
}