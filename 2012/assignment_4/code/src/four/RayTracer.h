#ifndef RAY_TRACER_H
#define RAY_TRACER_H

#include <cassert>
#include <vector>

#include "SceneParser.h"
#include "Ray.h"
#include "Hit.h"
#include "Group.h"

class SceneParser;


class RayTracer
{
public:
  
  RayTracer( SceneParser* s, int max_bounces );
  ~RayTracer();
  
  // You need to fill in the implementation for this function.
  Vec3f traceRay( Ray& ray, float tmin, int bounces, float refr_index, Hit& hit ) const;

private:

  Vec3f computeShadowColor( Ray& ray, float distanceToLight ) const;

  SceneParser*			m_scene;
  int					m_maxBounces;
  float					m_cutoffWeight;
};

#endif // RAY_TRACER_H
