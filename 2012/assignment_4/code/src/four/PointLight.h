#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "Light.h"

class PointLight : public Light
{
public:

    PointLight( const Vec3f& position, const Vec3f& intensity,
            float constantAttenuation,
            float linearAttenuation,
            float quadraticAttenuation );
    
    ~PointLight();

	// You need to fill in the implementation.
    virtual void getIncidentIllumination( const Vec3f& p,
            Vec3f& dir, Vec3f& col, float& distanceToLight ) const;

private:

    PointLight();

    Vec3f m_position;
    Vec3f m_intensity;

    float m_constantAttenuation;
    float m_linearAttenuation;
    float m_quadraticAttenuation;
};

#endif // POINT_LIGHT_H
