#include "Filter.h"

Filter::Filter()
{

}

// virtual
Filter::~Filter()
{

}

Filter* Filter::constructFilter( Args::ReconstructionFilterType t, float radius )
{
	if( t == Args::Filter_Box ) 
    {
        return new BoxFilter( radius );
    }
    else if ( t == Args::Filter_Tent ) 
    {
        return new TentFilter( radius );
    }
    else if ( t == Args::Filter_Gaussian ) 
	{
        return new GaussianFilter( radius );
    }
	else
	{
		::printf( "FATAL: Unknown reconstruction filter type!\n" );
		exit(0);
	}
}

BoxFilter::BoxFilter( float _radius )
{
    radius = _radius;
}


float BoxFilter::getSupportRadius() const
{
    return radius;
}

float BoxFilter::getWeight( const Vec2f& p ) const
{
    if( fabs( p.x ) <= radius &&
        fabs( p.y ) <= radius )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

TentFilter::TentFilter( float _radius )
{
    radius = _radius;
}

float TentFilter::getSupportRadius() const
{
    return radius;
}

float TentFilter::getWeight( const Vec2f& p ) const
{
	float answer;

	/*** YOUR CODE HERE (EXTRA)
	     Evaluate the origin-centered tent filter here.
	**/
    return answer;
}

GaussianFilter::GaussianFilter( float _sigma )
{
    sigma = _sigma;
    radius = 2.0f * sigma;
}

float GaussianFilter::getSupportRadius() const
{
    return radius;
}

float GaussianFilter::getWeight( const Vec2f& p ) const
{
    float answer;

    /*** YOUR CODE HERE (EXTRA)
	     Evaluate the origin-centered Gaussian filter here.
	**/

    return answer;
}

