#include "Sampler.h"

#include <cstdlib>

Sampler::Sampler( int nSamples ) :

    m_nSamples( nSamples )

{

}

// virtual
Sampler::~Sampler()
{

}

Sampler* Sampler::constructSampler( Args::SamplePatternType t, int numsamples )
{
	if( t == Args::Pattern_Uniform )
	{
		return new UniformSampler( numsamples );
	}
	else if ( t == Args::Pattern_Regular ) 
	{
		return new RegularSampler( numsamples );
	}
	else if ( t == Args::Pattern_Jittered ) 
	{
		return new JitteredSampler( numsamples );
	}
	else
	{
		::printf( "FATAL: Unknown sampler type!\n" );
		exit(0);
	}
}


UniformSampler::UniformSampler( int nSamples ) :    
    Sampler( nSamples )
{
    
}

// virtual
UniformSampler::~UniformSampler()
{

}

Vec2f UniformSampler::getSamplePosition( int i )
{
	// make the compiler shut up by referencing the variable
	(void)i;
	/*** YOUR CODE HERE
	     Return a uniformly distributed random 2-vector within the unit square [0,1]^2
	**/
	return this->m_generator.getVec2f(0.0f, 1.0f);
}

RegularSampler::RegularSampler( int nSamples ) :
    Sampler( nSamples )
{
    // test that it's a perfect square
    int dim = ( int )sqrtf( float( nSamples ) );
    assert( nSamples == dim * dim );
    m_dim = dim;
}

// virtual
RegularSampler::~RegularSampler()
{
}

Vec2f RegularSampler::getSamplePosition( int n )
{
	/*** YOUR CODE HERE
	     Return a sample through the center of the Nth subpixel.
		 The starter code only supports one sample per pixel.
	**/
	int row = n/this->m_dim;
	int col = n%this->m_dim;

	float step = (1.0f/this->m_dim);
	float offset = step/2.0f;
	return Vec2f(offset+(col*step), offset+(row*step));
}

JitteredSampler::JitteredSampler( int nSamples ) :
    
    Sampler( nSamples )
    
{
    // test that it's a perfect square
    int dim = ( int )sqrtf( float( nSamples ) );
    assert( nSamples == dim * dim );
    m_dim = dim;
}

// virtual
JitteredSampler::~JitteredSampler()
{

}


Vec2f JitteredSampler::getSamplePosition( int n )
{
	/*** YOUR CODE HERE
	     Return a randomly generated sample through Nth subpixel.
	**/
	int row = n/this->m_dim;
	int col = n%this->m_dim;

	float step = (1.0f/this->m_dim);
	float offset = step/2.0f;

	Vec2f subPixelOffset = this->m_generator.getVec2f(-offset, offset);

	return Vec2f(offset+subPixelOffset.x+(col*step), offset+subPixelOffset.y+(row*step));
}

