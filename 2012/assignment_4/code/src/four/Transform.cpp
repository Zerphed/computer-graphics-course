#include "Transform.h"

#include <cfloat>

#include "Ray.h"
#include "Hit.h"
#include "Triangle.h"
#include "VecUtils.h"

Transform::Transform( const Mat4f& m, Object3D* o ) :

    m_matrix( m ),
    m_object( o )

{
    assert( o != NULL );

    m_inverse = m_matrix.inverted();
    m_inverseTranspose = m_inverse.transposed();

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			m_inverseTranspose3x3.set(i, j, this->m_inverseTranspose.get(i, j));
		}
	}
}

// virtual
Transform::~Transform()
{

}

bool Transform::intersect( const Ray& r, Hit& h, float tmin ) const
{
	/*** YOUR CODE HERE
	     Transform the ray to the coordinate system of the object inside,
		 intersect, then transform the normal back. If you don't renormalize
		 the ray direction, you can just keep the t value and do not need to
		 recompute it!
		 Remember how points, directions, and normals are transformed differently!
	**/

	Vec3f origin_ws = r.getOrigin();
	Vec3f direction_ws = r.getDirection();

	// Transform the origin to OS: origin_os = M^-1 * origin_ws
	Vec3f origin_os = this->m_inverse * origin_ws;

	// Transform the direction to OS:
	// direction_os = M^-1 * (origin_ws + 1*direction_ws) - M^-1 * origin_ws
	Vec3f direction_os = this->m_inverse * (origin_ws + direction_ws) - origin_os;

	// Create a new ray to be used with the intersect method of the object
	Ray r_os(origin_os, direction_os);

	// Call the intersect routine of the object
	bool intersect = m_object->intersect(r_os, h, tmin);

	if (!intersect)
		return false;
	
	h.set(h.getT(), h.getMaterial(), (this->m_inverseTranspose3x3*h.getNormal()).normalized());
    return true;
}

