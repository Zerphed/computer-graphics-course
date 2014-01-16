#include "Args.h"

// the singleton
Args* Args::s_pInstance = 0;

void Args::parse( int argc, char *argv[] )
{
	defaultValues();

	for (int i = 1; i < argc; i++)
	{
		// rendering output
		if (!strcmp(argv[i],"-input")) {
			i++; assert (i < argc); 
			input_file = argv[i];
		} else if (!strcmp(argv[i],"-output")) {
			i++; assert (i < argc); 
			output_file = argv[i];
		} else if (!strcmp(argv[i],"-normals")) {
			i++; assert (i < argc); 
			normals_file = argv[i];
		} else if (!strcmp(argv[i],"-size")) {
			i++; assert (i < argc); 
			width = atoi(argv[i]);
			i++; assert (i < argc); 
			height = atoi(argv[i]);
		} else if (!strcmp(argv[i],"-stats")) {
			stats = 1;
		} 

		// rendering options
		else if (!strcmp(argv[i],"-depth")) {
			i++; assert (i < argc); 
			depth_min = (float)atof(argv[i]);
			i++; assert (i < argc); 
			depth_max = (float)atof(argv[i]);
			i++; assert (i < argc); 
			depth_file = argv[i];
		} else if (!strcmp(argv[i],"-bounces")) {
			i++; assert (i < argc); 
			bounces = atoi(argv[i]);
		} else if (!strcmp(argv[i],"-weight")) {
			i++; assert (i < argc); 
			weight = (float)atof(argv[i]);
		} else if (!strcmp(argv[i],"-transparent_shadows")) {
			shadows = 1;
			transparent_shadows = 1;
		} else if (!strcmp(argv[i],"-shadows")) {
			shadows = 1;
		} else if (!strcmp(argv[i],"-shade_back")) {
			shade_back = 1;
		}

		// Fresnel approximation
		else if (!strcmp(argv[i],"-fresnel")) {
			fresnel = 1;
		}

		// supersampling
		else if (!strcmp(argv[i],"-uniform_samples")) {
			sampling_pattern = Pattern_Uniform;
			i++; assert (i < argc); 
			num_samples = atoi(argv[i]);
		} else if (!strcmp(argv[i],"-regular_samples")) {
			sampling_pattern = Pattern_Regular;
			i++; assert (i < argc); 
			num_samples = atoi(argv[i]);
		} else if (!strcmp(argv[i],"-jittered_samples")) {
			sampling_pattern = Pattern_Jittered;
			i++; assert (i < argc); 
			num_samples = atoi(argv[i]);
		} else if (!strcmp(argv[i],"-box_filter")) {
			reconstruction_filter = Filter_Box;
			i++; assert (i < argc); 
			filter_radius = (float)atof(argv[i]);
			assert (filter_radius > 0);
		} else if (!strcmp(argv[i],"-tent_filter")) {
			reconstruction_filter = Filter_Tent;
			i++; assert (i < argc); 
			filter_radius = (float)atof(argv[i]);
			assert (filter_radius > 0);
		} else if (!strcmp(argv[i],"-gaussian_filter")) {
			reconstruction_filter = Filter_Gaussian;
			i++; assert (i < argc); 
			filter_radius = (float)atof(argv[i]);
			assert (filter_radius > 0);
		}

		// gui options
		else if (!strcmp(argv[i],"-gui")) {
			gui = 1;
		} else if (!strcmp(argv[i],"-tessellation")) {
			i++; assert (i < argc); 
			sphere_horiz = atoi(argv[i]);
			i++; assert (i < argc); 
			sphere_vert = atoi(argv[i]);
		} else if (!strcmp(argv[i],"-gouraud")) {
			gouraud_shading = 1;
		} else if (!strcmp(argv[i],"-specular_fix")) {
			specular_fix = 1;
		} else if (!strcmp(argv[i],"-show_progress")) {
			show_progress = 1;
		} else {
			::printf ("whoops error with command line argument %d: '%s'\n",i,argv[i]);
			assert(0);
		}
	}
}

void Args::defaultValues()
{
	// rendering output
	input_file = NULL;
	output_file = NULL;
	depth_file = NULL;
	normals_file = NULL;
	width = 100;
	height = 100;
	stats = 0;

	// rendering options
	depth_min = 0;
	depth_max = 1;
	bounces = 0;
	weight = 0.1;
	transparent_shadows = 0;
	shadows = 0;
	shade_back = 0;

	// Fresnel
	fresnel = 0;

	// sampling
	num_samples = 1;
	sample_zoom = 10;
	sampling_pattern = Pattern_Regular;
	reconstruction_filter = Filter_Box;
	filter_radius = 0.5;
	samples_file = NULL;
	filter_zoom = 10;
	filter_file = NULL;

	// gui options
	gui = 0;
	sphere_vert = 5;
	sphere_horiz = 10;
	gouraud_shading = 0;
	specular_fix = 0;

	show_progress = 1;
}
