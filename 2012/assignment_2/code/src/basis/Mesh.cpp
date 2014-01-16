#include "Mesh.h"

using namespace std;

void Mesh::load( const char* filename )
{
	// 2.1.1. load() should populate bindVertices, currentVertices, and faces
	// Add your code here.

	// Load the mesh from file into m_mesh.bindVertices.
	char buffer[ 255 ];
	ifstream meshFile( filename );
	while( meshFile.getline( buffer, 255 ) )
	{
		stringstream ss( buffer );

		string command;
		float x;
		float y;
		float z;

		ss >> command;
		if( command == "v" )
		{
			ss >> x >> y >> z;
			bindVertices.push_back( FW::Vec3f( x, y, z ) );
		}
		else if( command == "f" )
		{
			int v0;
			int v1;
			int v2;

			ss >> v0;
			ss >> v1;
			ss >> v2;
			faces.push_back( Tuple3u( v0, v1, v2 ) );
		}
	}

    meshFile.close();

	// make a copy of the bind vertices as the current vertices
	currentVertices = bindVertices;
}

void Mesh::draw()
{
	// Since these meshes don't have normals
	// be sure to generate a normal per triangle.
	// Notice that since we have per-triangle normals
	// rather than the analytical normals from
	// assignment 1, the appearance is "faceted".

	glBegin(GL_TRIANGLES);
	for (std::vector<Tuple3u>::iterator it = faces.begin(); it != faces.end(); it++) {
		// Calculate normal
		FW::Vec3f v0(currentVertices[(*it)[0]-1].x, currentVertices[(*it)[0]-1].y, currentVertices[(*it)[0]-1].z);
		FW::Vec3f v1(currentVertices[(*it)[1]-1].x, currentVertices[(*it)[1]-1].y, currentVertices[(*it)[1]-1].z);
		FW::Vec3f v2(currentVertices[(*it)[2]-1].x, currentVertices[(*it)[2]-1].y, currentVertices[(*it)[2]-1].z);
		FW::Vec3f normal = ((v1-v0).cross(v2-v1)).normalized();
		
		// Apply triangle normal
		glNormal3f(normal.x, normal.y, normal.z);

		// Draw vertices
		for (int idx = 0; idx < 3; idx++) {
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, vertexColors[(*it)[idx]-1].getPtr());
			glVertex3f(currentVertices[(*it)[idx]-1].x, currentVertices[(*it)[idx]-1].y, currentVertices[(*it)[idx]-1].z);
		}
	}

	glEnd();
}

void Mesh::loadAttachments( const char* filename, int numJoints )
{
	// 2.2. Implement this method to load the per-vertex attachment weights
	// this method should update m_mesh.attachments

	char buffer[ 1024 ];
	ifstream meshFile( filename );
	while( meshFile.getline( buffer, 1024 ) )
	{
		stringstream ss( buffer );

		vector<float> weights;
		weights.push_back(0);

		for(int boneNo = 0; boneNo < numJoints; boneNo++)
		{
			float weight;
			ss >> weight;
			weights.push_back(weight);
		}

		attachments.push_back(weights);
	}
    meshFile.close();

}
