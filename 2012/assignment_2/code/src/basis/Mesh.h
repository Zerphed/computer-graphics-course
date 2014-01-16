#ifndef MESH_H
#define MESH_H

#include <vector>
#include <base/Math.hpp>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include "tuple.h"
#include "extra.h"

typedef tuple< unsigned, 3 > Tuple3u;

struct Mesh
{
	// list of vertices from the OBJ file
	// in the "bind pose"
	std::vector< FW::Vec3f > bindVertices;

	// vertex colors
	std::vector< FW::Vec3f > vertexColors;

	// each face has 3 indices
	// referencing 3 vertices
	std::vector< Tuple3u > faces;

	// current vertex positions after animation
	std::vector< FW::Vec3f > currentVertices;

	// list of vertex to joint attachments
	// each element of attachments is a vector< float > containing
	// one attachment weight per joint
	std::vector< std::vector< float > > attachments;

	// 2.1.1. load() should populate bindVertices, currentVertices, and faces
	void load(const char *filename);

	// 2.1.2. draw the current mesh.
	void draw();

	// 2.2. Implement this method to load the per-vertex attachment weights
	// this method should update m_mesh.attachments
	void loadAttachments( const char* filename, int numJoints );
};

#endif
