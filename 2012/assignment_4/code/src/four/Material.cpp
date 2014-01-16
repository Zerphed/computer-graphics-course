#include "Material.h"
#include "Light.h"
#include "Args.h"
#include "VecUtils.h"

extern int DEBUG;

// virtual
Vec3f PhongMaterial::shade(const Ray &ray, const Hit &hit, 
        const Vec3f &dirToLight, 
        const Vec3f &incidentLightIntensity) const
{
	/*** YOUR CODE HERE
	     Implement the entire Phong reflectance model, including ambient,
		 diffuse, and specular, as described in the lecture notes.
		 NOTE: You should check the shade_back flag in Args, which
		 you can access using Args::instance(). If this flag is set,
		 you should treat the material as two-sided, i.e., flip the
		 normal if the ray hits the surface from behind. Otherwise
		 you should return zero for hits coming from behind the surface.
		 Remember, when computing the specular lobe, you shouldn't add
		 anything if the light is below the local horizon!
		 You should implement the "classic" Phong model, although
		 you're encouraged to play around with the halfvector version, too.
	**/

	Vec3f point = ray.pointAtParameter(hit.getT());
	Vec3f pointToEye = (ray.getOrigin()-point).normalized();
	Vec3f normal = hit.getNormal();
	Vec3f pixelColor(0.0f, 0.0f, 0.0f);

	if (Args::instance()->shade_back) {
		if (pointToEye.dot(normal) < 0.0f) {
			normal = -1.0f * normal;
		}
	}

	if (dirToLight.dot(normal) < 0.0f) {
		return pixelColor;
	}

	// 1. Ambient
	//    Not implemented here, comes into play from the traceRay function in the RayTracer class

	// 2. Diffuse
	//    Calculate the diffuse shading: c_light * clamp(L_i . N) * c_obj_diffuse
	pixelColor += incidentLightIntensity * FW::clamp(dirToLight.dot(normal), 0.0f, 1.0f) * this->getDiffuseColor(point);

	// 3. Specular
	//    Calculate the specular shading
	Vec3f R = ((2.0f*(pointToEye.dot(normal))*normal)-pointToEye).normalized();
	pixelColor += incidentLightIntensity * this->getSpecularColor() * FW::pow(FW::clamp(R.dot(dirToLight), 0.0f, 1.0f), this->getExponent());

    return pixelColor;
}

// virtual
Vec3f ProceduralMaterial::getDiffuseColor( const Vec3f& point ) const {
    Vec3f a1 = m_m1->getDiffuseColor(point);
    Vec3f a2 = m_m2->getDiffuseColor(point);
    Vec3f pt = VecUtils::transformPoint( m_matrix, point );
    float v = interpolation(pt);
    return a1 * v + a2 * (1 - v);
}

// virtual
Vec3f ProceduralMaterial::getReflectiveColor( const Vec3f& point) const {
    Vec3f a1 = m_m1->getReflectiveColor(point);
    Vec3f a2 = m_m2->getReflectiveColor(point);
    Vec3f pt = VecUtils::transformPoint( m_matrix, point );
    float v = interpolation(pt);
    return a1 * v + a2 * (1 - v);
}

// virtual
Vec3f ProceduralMaterial::getTransparentColor( const Vec3f& point) const {
    Vec3f a1 = m_m1->getTransparentColor(point);
    Vec3f a2 = m_m2->getTransparentColor(point);
    Vec3f pt = VecUtils::transformPoint( m_matrix, point );
    float v = interpolation(pt);
    return a1 * v + a2 * (1 - v);
}

// virtual
float ProceduralMaterial::getIndexOfRefraction( const Vec3f& point ) const {
    float a1 = m_m1->getIndexOfRefraction(point);
    float a2 = m_m2->getIndexOfRefraction(point);
    Vec3f pt = VecUtils::transformPoint( m_matrix, point );
    float v = interpolation(pt);
    return a1 * v + a2 * (1 - v);
}

// virtual
Vec3f ProceduralMaterial::shade(const Ray &ray, const Hit &hit, 
        const Vec3f &dirToLight, 
        const Vec3f &lightColor) const {
    Vec3f pt = ray.pointAtParameter(hit.getT());
    Vec3f a1 = m_m1->shade(ray,hit,dirToLight,lightColor);
    Vec3f a2 = m_m2->shade(ray,hit,dirToLight,lightColor);
    Vec3f pt2 = VecUtils::transformPoint( m_matrix, pt );
    float v = interpolation(pt2);
    Vec3f answer = a1 * v + a2 * (1 - v);
    return answer;
}

float Noise::interpolation( const Vec3f& point ) const 
{
	// noise
	return FW::clamp(this->perlinNoise->getNoise(point, 2.0f), 0.0f, 1.0f);
}

float Checkerboard::interpolation( const Vec3f& point ) const
{
    int count = 1;
    count += int(point[0]) % 2;
    count += int(point[1]) % 2;
    count += int(point[2]) % 2;
    if(point[0] < 0) count ++;
    if(point[1] < 0) count ++;
    if(point[2] < 0) count ++;
    return float(count & 0x01);
}


float Marble::interpolation( const Vec3f& point ) const {
	/*
	int nWidth = this->noise.getWidth();
	int nHeight = this->noise.getHeight();
	int nDepth = this->noise.getDepth();

	float turbulence = turbPower*this->noise.turbulence(point.x, point.y, point.z, turbSize)/256.0f;
	float xyzValue = point.x*xPeriod/nHeight + point.y*yPeriod/nWidth + point.z*zPeriod/nDepth + turbulence;
	float sineValue = 226.0f*FW::abs(FW::sin(xyzValue*3.14159f));
	*/

	return 0.0f;
}