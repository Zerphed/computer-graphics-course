#ifndef HIT_H
#define HIT_H

#include "Ray.h"

class Material;

class Hit {

public:

    // constructors
    Hit()
    {
        material	= NULL;
		t			= FLT_MAX;
    }

    Hit( float _t )
    { 
        t = _t;
		material	= NULL;
    }

    Hit( const Hit& h )
    { 
        t = h.t; 
        material = h.material; 
        normal = h.normal;
    }

    // destructor
    ~Hit()
    {
    
    }

    float getT() const
    {
        return t;
    }
    
    Material* getMaterial() const
    {
        return material;
    }
    
    const Vec3f& getNormal() const
    {
        return normal;
    }

    void set( float _t, Material* m, const Vec3f& n)
    {
        t = _t;
        material = m;
        normal = n;
    }

    float		t;			// closest hit found so far
    Material*	material;	// 
    Vec3f		normal;		//

};

inline ostream& operator << ( ostream &os, const Hit& h)
{
    os << "Hit <" << h.getT() << ", " << h.getNormal() << ">";
    return os;
}

#endif // HIT_H
