#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "base/Math.hpp"
using namespace FW;

#include "Object3D.h"

// A Transform wraps an object. When intersecting it with
// a ray, you need to transform the ray into the coordinate
// system "inside" the transform as described in the lecture.
class Transform : public Object3D
{
public:

    Transform( const Mat4f& m, Object3D* o );
    virtual ~Transform();

    bool intersect(const Ray &r, Hit &h, float tmin) const;  

private:

    Transform()
    {
        assert( false );
    }

    Mat4f m_matrix;
    Mat4f m_inverse;
    Mat4f m_inverseTranspose;
	Mat3f m_inverseTranspose3x3;
    Object3D* m_object;

};

#endif
