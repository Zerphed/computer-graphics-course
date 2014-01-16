#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "Light.h"

class DirectionalLight : public Light
{
public:

    DirectionalLight();
    
    DirectionalLight( const Vec3f& direction, const Vec3f& intensity );
    
    virtual ~DirectionalLight();

    virtual void getIncidentIllumination( const Vec3f& p,
            Vec3f& lightdir, Vec3f& lightIntensity, float& distanceToLight ) const;

private:

    Vec3f m_direction;
    Vec3f m_intensity;

};

#endif // DIRECTIONAL_LIGHT_H
