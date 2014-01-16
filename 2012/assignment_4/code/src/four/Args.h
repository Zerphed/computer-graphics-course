#ifndef ARG_PARSER_H
#define ARG_PARSER_H

#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>

class Args
{
public:

  Args() { defaultValues(); }

  // call this function to access the global singleton object
  static Args* instance()	{ if ( s_pInstance == 0 ) s_pInstance = new Args; return s_pInstance; }

 void parse( int argc, char *argv[] );
 void defaultValues();

  // ==============
  // REPRESENTATION
  // all public! (no accessors)

  // rendering output
  char*	input_file;
  char*	output_file;
  char*	depth_file;
  char*	normals_file;
  int	width;
  int	height;
  int	stats;

  // rendering options
  float depth_min;
  float depth_max;
  int	bounces;
  float weight; 
  int	transparent_shadows;
  int	shadows;
  int	shade_back;
  int   fresnel;

  // supersampling
  int	num_samples;
  int	sample_zoom;

  enum SamplePatternType
  {
	  Pattern_Regular	= 1,	// regular grid within the pixel
	  Pattern_Uniform	= 0,	// uniformly distributed random
	  Pattern_Jittered	= 2		// jittered within subpixels
  };

  SamplePatternType sampling_pattern;


  enum ReconstructionFilterType
  {
	  Filter_Box		= 0,
	  Filter_Tent		= 1,
	  Filter_Gaussian	= 2
  };
  ReconstructionFilterType	reconstruction_filter;

  float filter_radius;
  char*	samples_file;
  int	filter_zoom;
  char*	filter_file;

  // gui options
  int	gui;
  int	sphere_vert;
  int	sphere_horiz;
  int	gouraud_shading;
  int	specular_fix;
  int	show_progress;

private:
	static Args*	s_pInstance;
};

#endif // ARG_PARSER_H
