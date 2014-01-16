#ifndef CAMERA_H
#define CAMERA_H

#include <cassert>

#include "Ray.h"
#include "base/math.hpp"
#include <float.h>
#include <cmath>

using namespace FW;

class Camera
{
public:
	//generate rays for each screen-space coordinate
	virtual Ray generateRay( const Vec2f& point ) = 0 ; 

	/*** YOUR CODE HERE
		 Given floating-point pixel coordinates (px,py), you should return the corresponding normalized screen coordinates in [-1,1]^2
		 Pay attention to which direction is "up" :)
	**/
	static inline Vec2f normalizedImageCoordinateFromPixelCoordinate( const Vec2f& pixel, const Vec2i& imageSize )
	{
		float aspect = ((float)imageSize.x)/((float)imageSize.y);

		float dx = ( ( pixel.x/(imageSize.x*0.5f) ) - 1.0f )/aspect;
		float dy = ( 1.0f - ( pixel.y/(imageSize.y*0.5f) ) );

		return Vec2f(dx, dy);
	}
	
	virtual float getTMin() const = 0 ; 

protected:

	Vec3f center; 
	Vec3f direction;
	Vec3f up;
	Vec3f horizontal;
};


class OrthographicCamera: public Camera
{
public:
	OrthographicCamera( Vec3f center, Vec3f direction , Vec3f up , float size )
	{
		this->center = center;
		this->direction = direction.normalized();	
		this->horizontal = cross( direction , up ).normalized();
		// need to make an orthonormal vector to the projection
		this->up = cross(horizontal, direction).normalized(); 
		this->size = size;
	}

	/*** YOUR CODE HERE
		Generate a ray with the given screen coordinates, which you should assume lie in [-1,1]^2
	**/
	virtual Ray generateRay( const Vec2f& point )
	{
		// u = horizontal, v = up, w = direction
		// Origin = e + x*size*u + y*size*v
		float s = size/2.0f;
		Vec3f origin = this->center + point.x*s*this->horizontal + point.y*s*this->up;

		// Ray(origin, direction)
		return Ray(origin, this->direction);
	}

	virtual float getTMin() const
	{
		return -1.0f*FLT_MAX;
	}

private:
	float size ; 
};


class PerspectiveCamera: public Camera
{
public:
	PerspectiveCamera(const Vec3f& center_, const Vec3f& direction_,const Vec3f& up_ , float fov_angle)
	{
		this->center = center_;
		this->direction = direction_.normalized();
		this->horizontal = cross( direction_ , up_ ).normalized();
		this->up = cross(horizontal, direction_).normalized(); 
		this->fov_angle = fov_angle;
	}

	/*** YOUR CODE HERE
		Generate a ray with the given screen coordinates, which you should assume lie in [-1,1]^2
		How to do this is described in the lecture notes.
	**/
	virtual Ray generateRay( const Vec2f& point)
	{	
		// Image plane from -1 to 1, so size of one slice is 1
		float d = 1.0f/FW::tan((fov_angle/2.0f));
 
		// u = horizontal, v = up, w = direction
		// r = x*u + aspect*y*v + d*w
		Vec3f r = point.x*this->horizontal + point.y*this->up + d*this->direction;

		// Ray(origin, direction)
		return Ray(this->center, r.normalized());
	}

	virtual float getTMin() const { 
		return 0.0f;
	}

private:
	float fov_angle;
};

#endif //CAMERA_H


