#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <cassert>

#include "base/Math.hpp"
#include "Object3D.h"
#include "Plane.h"

using namespace FW;

class Triangle : public Object3D
{

public:

	// a triangle contains, in addition to the vertices, 2D texture coordinates,
	// often called "uv coordinates".
    Triangle( const Vec3f& a, const Vec3f& b, const Vec3f &c,
            Material* m,
            const Vec2f& ta = Vec2f( 0, 0 ),
            const Vec2f& tb = Vec2f( 0, 0 ),
            const Vec2f& tc = Vec2f( 0, 0 ) );

    virtual ~Triangle();

    bool intersect( const Ray &r, Hit &h, float tmin ) const;  

    const Vec3f& getVertex( int i ) const;

private:

    Triangle()
    {
        assert(0);
    }
    
    Vec3f vertices[3];
    Vec2f textureCoordinates[3];  
};

#endif
