#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <cassert>
// --------------------------------------------------------------------------

// Framework
#include "gui/Image.hpp"
#include "io/File.hpp"
#include "io/ImageLodePngIO.hpp"
// --------------------------------------------------------------------------

// our stuff
#include "Sphere.h"
#include "Group.h"
#include "Camera.h"
#include "Ray.h"
#include "Hit.h"
#include "SceneParser.h"
#include "Args.h"
#include "Light.h"
#include "Material.h"
#include "RayTracer.h"
#include "VecUtils.h"
#include "Film.h"
#include "Sampler.h"
#include "Filter.h"
// --------------------------------------------------------------------------

using namespace std;
using namespace FW;

// forward declaration for main()
void render( RayTracer* rt, SceneParser* scene, Args* args );
// --------------------------------------------------------------------------

// The main function: parse input and scene, render, quit
int main( int argc, char *argv[] )
{
	Args* args = Args::instance();

    // parse the command line arguments
    args->parse( argc, argv );
	// parse the scene
    SceneParser* scene   = new SceneParser( args->input_file );
	// construct tracer
    RayTracer* rayTracer = new RayTracer( scene, args->bounces );

	// render!
    render( rayTracer, scene, args );

	// clean up
	delete rayTracer;
    delete scene;
    delete args;

    return 0;
}
// --------------------------------------------------------------------------

// 
void render( RayTracer* rayTracer, SceneParser* scene, Args* args )
{
    assert( scene != NULL && args != NULL );

	Vec2i imageSize( args->width, args->height );

    // construct images
	Image* image = 0;
    Image* depth_image = 0;
	Image* normals_image = 0;
	if ( args->output_file != NULL )
    {
		image = new Image( imageSize, ImageFormat::RGBA_Vec4f );
        image->clear(Vec4f(0, 0, 0, 0));
    }
	if ( args->depth_file != NULL )
    {
		depth_image = new Image( imageSize, ImageFormat::RGBA_Vec4f );
        depth_image->clear(Vec4f(0, 0, 0, 0));
    }
    if ( args->normals_file != NULL )
    {
		normals_image = new Image( imageSize, ImageFormat::RGBA_Vec4f );
        normals_image->clear(Vec4f(0, 0, 0, 0));
    }

	// Construct sampler
	Sampler* sampler = Sampler::constructSampler( args->sampling_pattern, args->num_samples );

	/*** EXTRA
		The Filter and Film objects are not used by the starter code. They provide starting points
		for implementing smarter supersampling, whereas the required type of less fancy supersampling
		can be implemented by taking the average of the samples drawn from each pixel.
	**/
	Filter* filter = Filter::constructFilter( args->reconstruction_filter, args->filter_radius );
    Film* film = new Film( image, filter );

	/*** Main render loop!
		 Loop through all the pixels in the image
		 generate all the samples
		 fire rays
		 compute shading
		 accumulate into image
	*/
	// loop over scanlines
	for( int j = 0; j < args->height; ++j )
    {
		// print progress info
		if ( args->show_progress )
			::printf( "%.2f%%      \r", j*100.0f/imageSize.y );

		// loop over pixels on a scanline
		for( int i = 0; i < args->width; ++i )
        {
			Vec3f sampleColor(0.0f, 0.0f, 0.0f);

			// Loop through all the samples for this pixel.
            for( int n = 0; n < args->num_samples; ++n )
            {
				// Get the offset of the sample inside the pixel. 
				// You need to fill in the implementation for this function when implementing supersampling.
				// The starter implementation only supports one sample per pixel through the pixel center.
                Vec2f offset = sampler->getSamplePosition( n );

				// Convert floating-point pixel coordinate to canonical view coordinates in [-1,1]^2
				// You need to fill in the implementation for Camera::normalizedImageCoordinateFromPixelCoordinate.
				Vec2f ray_xy = Camera::normalizedImageCoordinateFromPixelCoordinate( Vec2f( float(i), float(j) ) + offset, imageSize );

				// Generate the ray using the view coordinates
				// You need to fill in the implementation for this function.
                Ray r = scene->getCamera()->generateRay( ray_xy );

				// trace the ray!
                Hit hit;
                float tmin = scene->getCamera()->getTMin();
				// !!Trace!!
				// You should fill in the gaps in the implementation of traceRay().
				// args->bounces gives the maximum number of reflections/refractions that should be traced.
                sampleColor += rayTracer->traceRay( r, tmin, args->bounces, 1.0f, hit );

				/*** YOUR CODE HERE
				     This starter code only supports one sample per pixel and consequently directly
					 puts the returned color to the image. You should extend the code to handle
					 multiple samples per pixel. The basic requirement is just to take an average
					 of all the samples within the pixel (so-called "box filtering"). Note that this
					 starter code does not take an average, it just assumes the first and only sample
					 is the final color.

					 For extra credit, you can implement more sophisticated ones, such as "tent" and bicubic
					 "Mitchell-Netravali" filters. This requires you to implement the addSample()
					 function in the Film class and use it instead of directly setting pixel values in the image.
				**/
            }

			image->setVec4f( Vec2i(i,j), Vec4f(sampleColor/args->num_samples,1.0f) );

			// if normal and depth images are requested, shoot special rays through the center,
			// i.e., do not supersample.
            if( depth_image != NULL || normals_image != NULL )
            {
				// Get canonical image coordinates from pixel units...
				Vec2f ray_xy = Camera::normalizedImageCoordinateFromPixelCoordinate( Vec2f( float(i), float(j) ) + Vec2f( 0.5f, 0.5f ), imageSize );
				// ..generate the ray..
                Ray r = scene->getCamera()->generateRay( ray_xy );

				// ..trace..
                Hit hit;
                float tmin = scene->getCamera()->getTMin();
                Vec3f shade = rayTracer->traceRay( r, tmin, args->bounces, 1.0f, hit );
				// ...and store results.
                if( depth_image != NULL )
                {
					/*** YOUR CODE HERE:
					     Here you should map the t range [depth_min, depth_max] to the inverted range [1,0] for visualization
						 Note that closer objects should appear brighter.
					**/
					float t = hit.getT();

					float dmax = Args::instance()->depth_max;
					float dmin = Args::instance()->depth_min;

					float f = 1.0f - ( FW::abs(t-dmin) / FW::abs(dmax-dmin) );
					
                    depth_image->setVec4f( Vec2i( i, j ), Vec4f( f, f, f, 1 ) );
                }
                if( normals_image != NULL )
                {
                    Vec3f n = hit.getNormal();
                    Vec3f color( fabs( n[0] ), fabs( n[1] ), fabs( n[2] ) );
					color = color.clamp( Vec3f(0), Vec3f(1) );
                    normals_image->setVec4f( Vec2i( i, j ), Vec4f( color, 1 ) );
                }
            } // close sample count
        } // close width
    } // close height

	/*** YOUR CODE HERE (EXTRA)
	     When you implement smarter filtering,
		 you should normalize the filter weight
		 carried in the 4th channel.
	**/
    if( image != NULL )
    {
    }

	// Clean up
    delete filter;
    delete sampler;
    delete film;

    // And finally, save the images as PNG!
    if( image != NULL )
    { 
		::printf( "Done, writing out %s\n", args->output_file );
		FW::File f( args->output_file, FW::File::Create );
		exportLodePngImage( f, image );
		delete image;
    }
    if( depth_image != NULL )
    { 
		FW::File f( args->depth_file, FW::File::Create );
		exportLodePngImage( f, depth_image );
        delete depth_image; 
    }
    if( normals_image != NULL)
    { 
		FW::File f( args->normals_file, FW::File::Create );
		exportLodePngImage( f, normals_image );
        delete normals_image; 
    }
}
