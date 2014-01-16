#ifndef MATERIAL_H
#define MATERIAL_H

#include <cassert>

#include "gui/Image.hpp"
#include "io/File.hpp"
#include "io/ImageTargaIO.hpp"

#include "Ray.h"
#include "Hit.h"

#include "PerlinNoise.h"

using namespace FW;

class Light;

class Material
{
public:

    Material()
    {
		memset( this, 0, sizeof(*this) );
    }

    Material( const Vec3f& diffuseColor,
            const Vec3f& reflectiveColor,
            const Vec3f& transparentColor,
			float indexOfRefraction,
            const char* textureTGAFilename ) :

        m_diffuseColor( diffuseColor ),
        m_reflectiveColor( reflectiveColor ),
        m_transparentColor( transparentColor ),
        m_indexOfRefraction( indexOfRefraction ),
        m_texture( NULL )
    {
        if( textureTGAFilename != NULL )
        {
			FW::File f( textureTGAFilename, FW::File::Read );
			m_texture = FW::importTargaImage( f );
        }
    }

    virtual ~Material()
    {
		// remember: it's ok to delete null pointers
        delete m_texture;
    }

    virtual Vec3f getDiffuseColor( const Vec3f& point ) const = 0;
    virtual Vec3f getReflectiveColor( const Vec3f& point ) const = 0;
    virtual Vec3f getTransparentColor( const Vec3f& point ) const = 0;
    virtual float getIndexOfRefraction( const Vec3f& point ) const = 0;

	// This function evaluates the light reflected at the point determined
	// by hit.t along the ray, in the direction of Ray, when lit from
	// light incident from dirToLight at the specified intensity.
    virtual Vec3f shade( const Ray& ray, const Hit& hit,
						 const Vec3f& dirToLight, const Vec3f& incidentLightIntensity ) const = 0;

protected:

    Vec3f m_diffuseColor;
    Vec3f m_reflectiveColor;
    Vec3f m_transparentColor;
    float m_indexOfRefraction;

    Image* m_texture;
};

// This class implements the Phong shading model.
class PhongMaterial : public Material
{
public:

    PhongMaterial( const Vec3f& diffuseColor,
            const Vec3f& specularColor, float exponent,
            const Vec3f& reflectiveColor,
            const Vec3f& transparentColor, float indexOfReflection,
            const char* textureTGAFilename ) :

        m_specularColor( specularColor ),
        m_exponent( exponent ),
        Material( diffuseColor,
                reflectiveColor,
                transparentColor, indexOfReflection,
                textureTGAFilename )
    {
    }
    
    virtual ~PhongMaterial()
    {
    
    }

    virtual Vec3f getDiffuseColor( const Vec3f& point ) const
    {
		(void)point;	// make the compiler shut up
        return m_diffuseColor;
    }
 
    virtual Vec3f getReflectiveColor( const Vec3f& point ) const
    {
		(void)point;	// make the compiler shut up
        return m_reflectiveColor;
    }
    
    virtual Vec3f getTransparentColor( const Vec3f& point ) const
    {
		(void)point;	// make the compiler shut up
        return m_transparentColor;
    }
   
    virtual float getIndexOfRefraction( const Vec3f& point ) const
    {
		(void)point;	// make the compiler shut up
        return m_indexOfRefraction;
    }
    
    Vec3f getSpecularColor() const
    {
        return m_specularColor;
    }
    
    float getExponent() const
    {
        return m_exponent;
    }

	// You need to fill in this implementation of this function.
    virtual Vec3f shade( const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor ) const;

private:

    PhongMaterial()
    {
        assert( false );
    }

    Vec3f m_specularColor;
    float m_exponent;

};


class ProceduralMaterial : public Material
{
public:

    ProceduralMaterial( const Mat4f& matrix, Material* m1, Material* m2 ) :

        m_matrix( matrix ),
        m_m1( m1 ),
        m_m2( m2 )

    {
        assert( m1 != NULL && m2 != NULL );
        m_matrix = matrix;
    }

    ~ProceduralMaterial()
    {

    }

    virtual Vec3f getDiffuseColor( const Vec3f& point ) const;
    virtual Vec3f getReflectiveColor( const Vec3f& point ) const;
    virtual Vec3f getTransparentColor( const Vec3f& point ) const;
    virtual float getIndexOfRefraction( const Vec3f& point ) const;

    virtual Vec3f shade( const Ray& ray, const Hit& hit, const Vec3f& dirToLight, const Vec3f& lightColor ) const;
    virtual float interpolation( const Vec3f& point ) const = 0;

protected:

    Mat4f m_matrix;
    Material* m_m1;
    Material* m_m2;

};


class Noise : public ProceduralMaterial
{

public:
	Noise( const Mat4f& matrix, Material* m1, Material* m2, int octaves) :
        ProceduralMaterial( matrix, m1, m2 ), octaves(octaves), perlinNoise(new PerlinNoise(octaves))
    {

    }

    ~Noise()
    {
		delete this->perlinNoise;
    }

    virtual float interpolation( const Vec3f& point ) const;

private:
	int octaves;
	PerlinNoise* perlinNoise;
};



class Checkerboard : public ProceduralMaterial
{
public:

    Checkerboard( const Mat4f& matrix, Material* m1, Material* m2 ) :

        ProceduralMaterial( matrix, m1, m2 )

    {

    }

    ~Checkerboard()
    {
    
    }

    virtual float interpolation( const Vec3f& point ) const;

};


class Marble : public ProceduralMaterial
{

public:
	Marble( const Mat4f& matrix, Material* m1, Material* m2, int octaves, 
		float frequency, float amplitude) :

    ProceduralMaterial( matrix, m1, m2 ), octaves(octaves), frequency(frequency),
			amplitude(amplitude), perlinNoise(new PerlinNoise(octaves, frequency, amplitude))
    {

    }

    ~Marble()
    {
		delete this->perlinNoise;
    }

    virtual float interpolation( const Vec3f& point ) const;

private:
	int octaves, frequency, amplitude;
	PerlinNoise* perlinNoise;
};

#endif // MATERIAL_H
