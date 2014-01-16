#ifndef OBJECT_3D_H
#define OBJECT_3D_H

#include <vector>
 
#include "Ray.h"
#include "Hit.h"

class Material;

// This is the base class for the all the kinds of objects in the scene.
// Its subclasses are Groups, Transforms, Triangles, Planes, Spheres, etc.
// The base class defines functions for intersecting rays with the contents.
class Object3D
{
public:

    Object3D();
    Object3D( Material* m );
    virtual ~Object3D();

	// Intersect ray with the object.
	// You need to fill in the implementations for several types of objects.
	// Hit is the structure that maintains information about the closest hit found
	// so far. Tmin defines the starting point for the ray, which you can use
	// for dealing with those pesky epsilon issues.
    virtual bool intersect( const Ray& r, Hit& h, float tmin) const = 0;

	// 
    virtual Material* getMaterial() const;
    void setMaterial( Material* m );

protected:

  Material*					m_material;

};

#endif // OBJECT_3D_H
