#include "RayTracer.h"
#include "Camera.h"
#include "Ray.h"
#include "Hit.h"
#include "Group.h"
#include "Material.h"
#include "Light.h"
#include "Args.h"

#define EPSILON 0.01f
#define sq(x) ((x)*(x))
#define pow5(x) ((x)*(x)*(x)*(x)*(x))

using namespace std;

/*** YOUR CODE HERE
     Compute the mirror direction for the incoming direction, given the surface normal.
	 Pay attention to the direction which things point towards, and that you only
	 pass in normalized vectors.
**/
Vec3f inline mirrorDirection( const Vec3f& normal, const Vec3f& incoming)
{
	// Mirrored direction: -2*(V dot N)*N + V
    return (-2.0f*(incoming.dot(normal))*normal + incoming).normalized();
}

/*** YOUR CODE HERE (EXTRA)
     Compute the transmitted direction for the incoming direction, given the surface normal
	 and indices of refraction. Pay attention to the direction which things point towards!
	 You should only pass in normalized vectors!
	 The function should return true if the computation was successful, and false
	 if the transmitted direction can't be computed due to total internal reflection.
**/
bool inline transmittedDirection( const Vec3f& normal, const Vec3f& incoming, 
        float index_i, float index_t, Vec3f& transmitted)
{
	// Calculate angle of refraction
	float cosI = -(normal.dot(incoming));

	// Coming from inside the object to outside
	bool inside = false;
	if (cosI < 0.0f) {
		index_t = 1.0f;
		inside = true;
	}

	float n = index_i/index_t;
	float sinO2 = sq(n) * (1.0f - cosI*cosI);

	// TIR
	if (sinO2 > 1.0f) {
		return false;
	} 

	float cosT = FW::sqrt(1.0f - sinO2);
	if (inside) {
		transmitted = (n * incoming + (n*cosI+cosT) * normal).normalized();
	}
	else {
		transmitted = (n * incoming + (n*cosI-cosT) * normal).normalized();
	}

	return true;
}


void inline getFresnelCoefficients(const Vec3f& normal, const Vec3f& direction, 
	float n1, float n2, float& R, float& T) {
	
	float cosOi = -(normal.dot(direction));

	// Check for the possibility of coming from inside the object to outside
	if (cosOi < 0.0f) {
		n2 = 1.0f;
	}

	float R0 = sq(((n1-n2)/(n1+n2)));

	if (n1 >= n2) {
		float n = n1/n2;
		float sinO2 = sq(n) * (1.0f - sq(cosOi));
		
		// TIR
		if (sinO2 > 1.0f) {
			R = 1.0f;
			T = 0.0f;
			return;
		}

		cosOi = FW::sqrt(1.0f - sinO2);
	}
	
	float x = 1.0f - cosOi;
	R = R0 + (1.0f - R0) * pow5(x);
	T = 1.0f - R;
}

Vec3f inline getEpsilonVector(const Vec3f& point, const Vec3f& normal) {
	return FLT_EPSILON*FW::max(1.0f, point.length())*normal*100.0f;
}


RayTracer::RayTracer( SceneParser* s, int max_bounces ) :
    m_scene( s )
{
    // rayTree = new RayTree();
    m_maxBounces = max_bounces;
}

RayTracer::~RayTracer()
{
}

// 

Vec3f RayTracer::traceRay( Ray& ray, float tmin, int bounces,
        float refr_index, Hit& hit ) const
{
	// initialize a hit to infinitely far away
    hit = Hit( FLT_MAX );

	// ask the root node (the single "Group" in the scene)
	// to get the intersection. You need to fill in its implementation.
	bool intersect = m_scene->getGroup()->intersect( ray, hit, tmin );

	// if the ray misses, we'll return this.
    Vec3f answer = m_scene->getBackgroundColor();

    if (intersect)
    {
		Material* m = hit.getMaterial();
        assert( m != NULL );

		// Get the intersection point, direction and normal.
		const Vec3f& direction = ray.getDirection().normalized();
        const Vec3f& normal = hit.getNormal().normalized();
        const Vec3f& point = ray.pointAtParameter( hit.getT() );
		bool inside = dot( normal,direction ) > 0.0f;

        // Ambient light
		/*** YOUR CODE HERE
			 You should replace the "answer" with ambient light from the scene, modulated by the diffuse color.
		**/
		answer = this->m_scene->getAmbientLight() * m->getDiffuseColor(point);

		/*** YOUR CODE HERE
			 Here, you should loop over all the lights in the scene,
			 fire shadow rays towards them, and add the contributions
			 of the lights if the shadow rays tell you the light is not blocked.
			 You need to check the Args structure; if it says no shadows,
			 you shouldn't cast the shadow ray. The lights should still light
			 the object, though :)

             You can ask whether we want shadows with
        **/
		int numLights = this->m_scene->getNumLights();

		// The pass-by-reference parameters for the getIncidentIllumination method
		Vec3f lightdir(0.0f, 0.0f, 0.0f);
		Vec3f incidentIntensity(0.0f, 0.0f, 0.0f);
		float distanceToLight = 0.0f;

		if (Args::instance()->shadows)
        {
			// Yes, handle shadows.
			for (int i = 0; i < numLights; i++) {
				this->m_scene->getLight(i)->getIncidentIllumination(point, lightdir, incidentIntensity,
																	distanceToLight);

				Vec3f epsilonDisplacement = getEpsilonVector(point, normal);
				Ray sr(point+epsilonDisplacement, lightdir);
				Hit sh = (FLT_MAX);

				bool inShadow = m_scene->getGroup()->intersectShadows(sr, sh, EPSILON, distanceToLight);
				if (!inShadow) {
					answer += m->shade(ray, hit, lightdir, incidentIntensity);
				}
			}
        }
		else {
			for (int i = 0; i < numLights; i++) {
				this->m_scene->getLight(i)->getIncidentIllumination(point, lightdir, incidentIntensity,
																	distanceToLight);
				answer += m->shade(ray, hit, lightdir, incidentIntensity);
			}
		}


		// Are there bounces left?
        if( bounces >= 1 )
        {
			// Fresnel coefficients
			float R = 1.0f, T = 1.0f;

			 // refraction, but only if surface is transparent!
			if (m->getTransparentColor( point ).length() > 0.0f )
            {
				/*** YOUR CODE HERE (EXTRA)
				     Generate a refracted direction and trace the ray. For this, you need
					 the index of refraction of the object. You should consider a ray going through
					 the object "against the normal" to be entering the material, and a ray going
					 through the other direction as exiting the material to vacuum (refractive index=1).
					 (Assume rays always start in vacuum, and don't worry about multiple intersecting
					 refractive objects!) Remembering this will help you figure out which way you
					 should use the material's refractive index. Remember to modulate the result
					 with the material's refractiveColor().
					 REMEMBER you need to account for the possibility of total internal reflection as well.
				**/
				if (Args::instance()->fresnel) {
					getFresnelCoefficients(normal, direction, refr_index, m->getIndexOfRefraction(point), R, T);
				}

				Vec3f refractedDir;
				Hit refractedHit;
				Vec3f epsilonDisplacement = -getEpsilonVector(point, normal);
				float refr_index2 = m->getIndexOfRefraction(point);

				// Calculate the transmitted direction
				bool transmitted = transmittedDirection(normal, direction, refr_index, refr_index2, refractedDir);

				if (transmitted) { // If refraction occurs

					// If we're inside the object reverse the epsilon displacement
					if (inside) {
						refr_index2 = 1.0f;
						epsilonDisplacement *= -1.0f;
					}

					Ray refractedRay(point+epsilonDisplacement, refractedDir);
					Vec3f refractedColor = traceRay(refractedRay, EPSILON, bounces-1, refr_index2, refractedHit);

					answer += T * refractedColor * m->getTransparentColor(point);
				}
				else { // TIR
					Ray reflectedRay(point+epsilonDisplacement, mirrorDirection(-normal, direction));
					Vec3f reflectedColor = traceRay(reflectedRay, EPSILON, bounces-1, refr_index2, refractedHit);
					answer += R * reflectedColor * m->getReflectiveColor(point);
				}
				
            }


            // reflection, but only if reflective coefficient > 0! and we're NOT INSIDE THE OBJECT
			if ( m->getReflectiveColor( point ).length() > 0.0f)
            {
				/*** YOUR CODE HERE
				     Generate and trace a reflected ray to the ideal mirror direction and add
					 the contribution to the result. Remember to modulate the returned light
					 by the reflective color of the material of the hit point.
				**/

				// Calculate the reflected direction: ray direction reflected against the point normal
				Vec3f reflectedDir = mirrorDirection(normal, direction);

				// Create a new reflected ray and a new hit
				Vec3f epsilonDisplacement = getEpsilonVector(point, normal);
				Ray reflectedRay(point+epsilonDisplacement, reflectedDir);
				Hit reflectedHit;

				Vec3f reflectedColor = traceRay(reflectedRay, EPSILON, bounces-1, refr_index, reflectedHit);
				
				answer += R * reflectedColor * m->getReflectiveColor(point);
            }
        }
    }

    return answer;
}
