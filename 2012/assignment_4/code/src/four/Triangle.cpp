#include "Triangle.h"

#include <GL/gl.h>
#include <GL/glu.h>

#include "Material.h"
#include "VecUtils.h"

Triangle::Triangle( const Vec3f& a, const Vec3f& b, const Vec3f& c,
        Material *m,
        const Vec2f& ta, const Vec2f& tb, const Vec2f& tc ) :
    Object3D( m )
{
    vertices[0] = a;
    vertices[1] = b;
    vertices[2] = c;

    textureCoordinates[0] = ta;
    textureCoordinates[1] = tb;
    textureCoordinates[2] = tc;
}

Triangle::~Triangle()
{
}

bool Triangle::intersect( const Ray& r, Hit& h, float tmin ) const
{
	/*** YOUR CODE HERE
	     Intersect the triangle with the ray!
		 Again, pay attention to respecting tmin and h.t!
	**/

	// Get origin and direction
	Vec3f direction = r.getDirection();
	Vec3f origin = r.getOrigin();

	// Construct the "supporting plane"
	Vec3f a = this->getVertex(0);
	Vec3f b = this->getVertex(1);
	Vec3f c = this->getVertex(2);

	Vec3f ba = b-a;
	Vec3f ca = c-a;

	Vec3f n = (ba.cross(ca)).normalized();
	float d = n.dot(a);

	// Get the intersection point Q
	float nd = n.dot(direction);

	// If the dot product is zero the ray and the triangle are parallel
	if (nd == 0.0f)
		return false;

	float t = (d - n.dot(origin)) / (n.dot(direction));
	Vec3f q = r.pointAtParameter(t);

	// Triangle inside-outside testing
	float abq = ((b-a).cross(q-a)).dot(n);
	if (abq < 0.0f)
		return false;

	float qbc = ((c-b).cross(q-b)).dot(n);
	if (qbc < 0.0f)
		return false;

	float aqc = ((a-c).cross(q-c)).dot(n);
	if (aqc < 0.0f)
		return false;

	if (t > tmin && t < h.getT()) {
		h.set(t, this->getMaterial(), n);
		return true;
	}
	
	return false;
}

const Vec3f& Triangle::getVertex( int i ) const
{
    assert( i >= 0 && i < 3 );
    return vertices[i];
}


