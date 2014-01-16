#ifndef BOX_H
#define BOX_H

#include "Object3D.h"

#include "base/math.hpp"

class Box : public Object3D
{
public:
  
    Box( const Vec3f& _min, const Vec3f& _max, Material* m );
    virtual ~Box();

    bool intersect( const Ray& r, Hit& h, float tmin ) const;

private:
	Vec3f	m_min;
	Vec3f	m_max;
};

#endif
