#ifndef RAY_H
#define RAY_H

#include <cassert>
#include <iostream>
#include "base/math.hpp"

using namespace FW;
using namespace std;

// Ray class mostly copied from Peter Shirley and Keith Morley
class Ray
{
public:

    Ray( const Vec3f& orig, const Vec3f& dir )
    {
        origin = orig; 
        direction = dir;
    }

    Ray( const Ray& r )
    { 
        origin = r.origin;
        direction = r.direction;
    }

    const Vec3f& getOrigin() const
    {
        return origin;
    }

    const Vec3f& getDirection() const
    {
        return direction;
    }
    
    Vec3f pointAtParameter( float t ) const
    {
        return origin + direction * t;
    }

private:

    // don't use this constructor
    Ray()
    {
        assert( false );
    }

    Vec3f origin;
    Vec3f direction;

};

inline ostream& operator << ( ostream& os, const Vec3f& v)
{
	os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
}

inline ostream& operator << ( ostream& os, const Vec2f& v)
{
	os << "[" << v.x << ", " << v.y << "]";
}

inline ostream& operator << ( ostream& os, const Ray& r)
{
    os << "Ray <" << r.getOrigin() << ", " << r.getDirection() << ">";
    return os;
}

#endif // RAY_H
