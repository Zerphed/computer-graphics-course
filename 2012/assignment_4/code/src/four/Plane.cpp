#include "Plane.h"

#include <GL/gl.h>

#include "SceneParser.h"
#include "Material.h"
#include "Group.h"

extern SceneParser *scene;

#define EPSILON 0.000001f
#define PLANE_SIZE 10

Plane::Plane( const Vec3f& normal, float d, Material* m ) :
    
    m_normal( normal.normalized() ),
    m_d( d ),
    Object3D( m )

{ 

}

// virtual
Plane::~Plane()
{

}

bool Plane::intersect( const Ray& r, Hit& h, float tmin ) const
{
	float nd = this->m_normal.dot(r.getDirection());
	if (nd == 0.0f) {
		return false;
	}

	float t = (this->m_d - (this->m_normal.dot(r.getOrigin())))/nd;

	if (t < h.getT() && t > tmin) {
		h.set(t, this->m_material, this->m_normal);
		return true;
	}

	return false;
}

const Vec3f& Plane::getNormal() const
{
    return m_normal;
}

float Plane::getOffset() const
{
    return m_d;
}

Vec3f PtOnPlaneX(float y, float z, const Vec3f &normal, float d) {
  float x = (d - y*normal[1] - z*normal[2]) / normal[0];
  return Vec3f(x,y,z);
}

Vec3f PtOnPlaneY(float x, float z, const Vec3f &normal, float d) {
  float y = (d - x*normal[0] - z*normal[2]) / normal[1];
  return Vec3f(x,y,z);
}

Vec3f PtOnPlaneZ(float x, float y, const Vec3f &normal, float d) {
  float z = (d - y*normal[1] - x*normal[0]) / normal[2];
  return Vec3f(x,y,z);
}
