#include "PointLight.h"

PointLight::PointLight( const Vec3f& position, const Vec3f& intensity,
        float constantAttenuation,
        float linearAttenuation,
        float quadraticAttenuation ) :

    m_position( position ),
    m_intensity( intensity ),
    m_constantAttenuation( constantAttenuation ),
    m_linearAttenuation( linearAttenuation ),
    m_quadraticAttenuation( quadraticAttenuation )

{

}
    
PointLight::~PointLight()
{

}

// virtual
void PointLight::getIncidentIllumination( const Vec3f& p,
        Vec3f& lightdir, Vec3f& incidentLight, float& distanceToLight ) const
{
	/*** YOUR CODE HERE
		 Evaluate the incident illumination at point p, accounting
		 for distance attenuation. Distance attenuation should be
		 computed using 1/(ar^2+br+c), with a,b,c given by the members
		 m_quadraticAttenuation, m_linearAttenuation, and m_constantAttenuation.
		 To facilitate the implementation of Phong, do not take the
		 incident cosine into account here.
	**/
	// Vector from point to light source
	Vec3f pl = (this->m_position - p);
	distanceToLight = pl.length();
	lightdir = pl.normalized();

	float ar = this->m_quadraticAttenuation*distanceToLight*distanceToLight;
	float br = this->m_linearAttenuation*distanceToLight;
	float c  = this->m_constantAttenuation;

	incidentLight = this->m_intensity/(ar+br+c);
}

