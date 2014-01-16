#include "Box.h"

#define EPSILON 0.0001f
#define sq(x) ((x)*(x))

Box::Box( const Vec3f& _min, const Vec3f& _max, Material* m ) :
    
    Object3D( m ), m_min(_min), m_max(_max)

{

}

// virtual
Box::~Box()
{
    
}

bool Box::intersect( const Ray& r, Hit& h, float tmin ) const
{
	float maxToCam = (r.getOrigin() - this->m_max).length();
	float minToCam = (r.getOrigin() - this->m_min).length();

	Vec3f direction = r.getDirection();
	Vec3f origin = r.getOrigin();

	// Keep the direction values sane, we don't want to divide by zero
	if (direction.x == 0.0f)
		direction.x += EPSILON;
	if (direction.y == 0.0f)
		direction.y += EPSILON;
	if (direction.z == 0.0f)
		direction.z += EPSILON;

	float min, max, ymin, ymax, zmin, zmax;

	// Calculate the maximum and minimum on the x-axis
	float divx = 1.0f/direction.x;
	if (divx >= 0) {
		min = (this->m_min.x - origin.x) * divx;
		max = (this->m_max.x - origin.x) * divx;
	}
	else {
		min = (this->m_max.x - origin.x) * divx;
		max = (this->m_min.x - origin.x) * divx;
	}

	// Calculate the maximum and minimum on the y-axis
	float divy = 1.0f/direction.y;
	if (divy >= 0) {
		ymin = (this->m_min.y - origin.y) * divy;
		ymax = (this->m_max.y - origin.y) * divy;
	}
	else {
		ymin = (this->m_max.y - origin.y) * divy;
		ymax = (this->m_min.y - origin.y) * divy;
	}

	if ( (min > ymax) || (ymin > max) )
		return false;

	// Make sure the tmin and tmax hold the most binding values
	if (ymin > min)
		min = ymin;
	if (ymax < max)
		max = ymax;

	// Calculate the maximum and minimum on the z-axis
	float divz = 1.0f/direction.z;
	if (divz >= 0) {
		zmin = (this->m_min.z - origin.z) * divz;
		zmax = (this->m_max.z - origin.z) * divz;
	}
	else {
		zmin = (this->m_max.z - origin.z) * divz;
		zmax = (this->m_min.z - origin.z) * divz;
	}

	if ( (min > zmax) || (zmin > max ) )
		return false;
	
	// Make sure the tmin and tmax hold the most binding values
	if (zmin > min)
		min = zmin;
	if (zmax < max)
		max = zmax;

	if ( (min < h.getT() && (max > tmin)) ) {
		// Calculate the normal and set the values
		Vec3f point = r.pointAtParameter(min);
		Vec3f normal;

		if (FW::abs(point.x - this->m_min.x) < EPSILON)
			normal = Vec3f(-1.0f, 0.0f, 0.0f);
		else if (FW::abs(point.x - this->m_max.x) < EPSILON)
			normal = Vec3f(1.0f, 0.0f, 0.0f);
		else if (FW::abs(point.y - this->m_min.y) < EPSILON)
			normal = Vec3f(0.0f, -1.0f, 0.0f);
		else if (FW::abs(point.y - this->m_max.y) < EPSILON)
			normal = Vec3f(0.0f, 1.0f, 0.0f);
		else if (FW::abs(point.z - this->m_min.z) < EPSILON)
			normal = Vec3f(0.0f, 0.0f, -1.0f);
		else if (FW::abs(point.z - this->m_max.z) < EPSILON)
			normal = Vec3f(0.0f, 0.0f, 1.0f);
		else
			std::cerr << "ERROR: Could not calculate the normal in box intersect." << std::endl;

		h.set(min, this->m_material, normal);
		return true;
	}

	return false;
} 

