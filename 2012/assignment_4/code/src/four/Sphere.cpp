#include "Sphere.h"

#include "Material.h"
#include "Args.h"

Sphere::Sphere( const Vec3f& center, float radius, Material* m ) :

    m_center( center ),
    m_radius( radius ),
    Object3D( m )

{ 
    assert( radius >= 0 );

    Vec3f min = center - Vec3f( radius, radius, radius );
    Vec3f max = center + Vec3f( radius, radius, radius );
}

// virtual
Sphere::~Sphere()
{
}

bool Sphere::intersect( const Ray& r, Hit& h, float tmin ) const
{
	/*** YOUR CODE HERE
	     Fill in the code to intersect the ray with the sphere.
		 Make sure you respect tmin and the closest hit distance in hit.
		 Also, observe that the sphere is not necessarily centered at the origin!
	**/

	// Transform to object space (translate origin and direction)
	Vec3f ro = r.getOrigin()-this->m_center;
	Vec3f rd = r.getDirection();

	// Implicit sphere equation:
	// H(P) = ||P||^2 - r^2 = P.P - r^2 = 0
	
	// Explicit ray equation:
	// P(t) = R_0 ' t*R_d, where R_0 = origin, R_d = direction

	// Quadratic: at^2 + bt + c = 0
	// a = 1 (remember, ||R_d|| = 1)
	// b = 2*R_d . R_0
	// c = R_0 . R_0 - r^2

	// Discriminant
	// d = sqrt(b^2 - 4ac)

	// Solutions
	// t+- = (-b +- d)/2a

	float a = 1.0f;
	float b = 2.0f*rd.dot(ro);
	float c = ro.dot(ro) - (this->m_radius*this->m_radius);
	float discr = b*b - (4*a*c);

	// No real roots, therefore there cannot be any intersections
	if (discr < 0.0f) {
		return false;
	}

	// Calculate the square root now that we know the discriminant isn't negative
	float d = FW::sqrt(discr);
	
	float q;
	if (b < 0.0f) {
		q = ((-b)-d)/2.0f;
	}
	else {
		q = ((-b)+d)/2.0f;
	}

	float t0 = q/a;
	float t1 = c/q;

	// The intersection is at the smallest positive point, check that it fulfills
	// the requirements imposed by the tmin and current h.t (closest hit)
	float intersection;
	if (t0 < t1 && t0 > tmin && t0 < h.getT()) {
		intersection = t0;
	}
	else if (t1 > tmin && t1 < h.getT()) {
		intersection = t1;
	}
	else {
		return false;
	}

	h.set(intersection, this->m_material, (r.pointAtParameter(intersection)-this->m_center).normalized());
	return true;
} 

