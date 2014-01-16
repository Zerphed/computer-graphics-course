#ifndef LIGHT_H
#define LIGHT_H

#include <cfloat>

#include "Object3D.h"

// Abstract base class for lights.
class Light
{
public:

    Light();
    virtual ~Light();
    
	// This function evaluates, at the point p in the scene,
	// what direction the light is at, what its intensity
	// is at the shading point, and how far away the light is.
	// The distance is only used for shadow rays, actual attenuation
	// due to distance should be accounted for in col.
	// To facilitate the implementation of Phong, do not take the
	// incident cosine into account here.
    virtual void getIncidentIllumination( const Vec3f& p, Vec3f& lightdir, Vec3f& incidentIntensity, float& distanceToLight ) const = 0;

};


#endif // LIGHT_H
