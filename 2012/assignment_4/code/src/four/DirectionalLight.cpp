#include "DirectionalLight.h"

DirectionalLight::DirectionalLight() :

    m_direction( 0, 0, 0 ),
    m_intensity( 1, 1, 1 )

{

}

DirectionalLight::DirectionalLight( const Vec3f& direction, const Vec3f& intensity ) :

    m_direction( direction.normalized() ),
    m_intensity( intensity )
{

}

DirectionalLight::~DirectionalLight()
{

}

// virtual
void DirectionalLight::getIncidentIllumination( const Vec3f& p,
        Vec3f& lightdir, Vec3f& incidentIntensity, float& distanceToLight ) const
{
	/***
		YOUR CODE HERE
		You should fill in the appropriate information about the "infinitely distant" light
		for the renderer to use. See the comment in the base class for extra information.
	**/
	lightdir = (-this->m_direction).normalized();
	incidentIntensity = this->m_intensity;
	distanceToLight = FLT_MAX;
}
