#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"

class Sphere : public Object3D
{
public:

    Sphere( const Vec3f& center, float radius, Material* m );
    virtual ~Sphere();

    bool intersect( const Ray& r, Hit& h, float tmin ) const;

private:

    Sphere()
    {
        assert( false );
    }

    Vec3f m_center;
    float m_radius;

};

#endif // SPHERE_H
