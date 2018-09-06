#pragma once

#include "Hitable.h"
#include "Ray.h"

#include <vector>

class HitableList : public Hitable
{
public:
    ~HitableList();
    virtual bool hit(const Ray& r, float min, float max, Hit& hit) const;

    template<typename T, typename... Args>
    T* addHitable(Args... args)
    {
        m_list.push_back(new T(args...));
        return static_cast<T*>(m_list.back());
    }

private:
    std::vector<Hitable*> m_list;
};
