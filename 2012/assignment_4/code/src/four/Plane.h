#ifndef PLANE_H
#define PLANE_H

#include "Object3D.h"

class Plane : public Object3D
{
public:

    Plane( const Vec3f& normal, float d, Material* m );
    virtual ~Plane();

    bool intersect( const Ray& r, Hit& h, float tmin ) const;  

    const Vec3f& getNormal() const;
    float getOffset() const;

private:

    Plane()
    {
        assert( false );
    }

    Vec3f m_normal;
    float m_d;

};

#endif // PLANE_H
