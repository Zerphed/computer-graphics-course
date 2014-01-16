#ifndef SKELETALMODEL_H
#define SKELETALMODEL_H
/*
#ifdef WIN32
#include <windows.h>
#endif
*/
#ifndef M_PI
#define M_PI 3.14159265358979f
#endif

#include <cstdlib>
//#include <FL/gl.h>
//#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include <base/Math.hpp>

#include "tuple.h"
#include "Joint.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "extra.h"

class SkeletalModel
{
public:

    ~SkeletalModel (void);

	// Already-implemented utility functions that call the code you will write.
	void load(const char *skeletonFile, const char *meshFile, const char *attachmentsFile);
	void draw(FW::Mat4f cameraMatrix, bool drawSkeleton);

	// Part 1: Understanding Hierarchical Modeling

	// 1.1. Implement method to load a skeleton.
	// This method should compute m_rootJoint and populate m_joints.
	void loadSkeleton( const char* filename );

	// 1.1. Implement this method with a recursive helper to draw a sphere at each joint.
	void drawJoints( );
	void drawJointsRecurse( Joint* node );


	// 1.2. Implement this method a recursive helper to draw a box between each pair of joints
	void drawSkeleton( );
	void drawSkeletonRecurse( Joint* node );

	// 1.3. Implement this method to handle changes to your skeleton given
	// changes in the slider values
	void setJointTransform( int jointIndex, float rX, float rY, float rZ );

	// Part 2: Skeletal Subspace Deformation

	// 2.3. Implement SSD

	// 2.3.1. Implement this method to compute a per-joint transform from
	// world-space to joint space in the BIND POSE.
	void computeBindWorldToJointTransforms();
	void computeBindWorldToJointTransformsRecurse(Joint *joint, FW::Mat4f currentMatrix);

	void computeJointColors();
	void SkeletalModel::computeVertexColors();

	// 2.3.2. Implement this method to compute a per-joint transform from
	// joint space to world space in the CURRENT POSE.
	void updateCurrentJointToWorldTransforms();
	void updateCurrentJointToWorldTransformsRecurse(Joint *joint, FW::Mat4f currentMatrix);

	// 2.3.2. This is the core of SSD.
	// Implement this method to update the vertices of the mesh
	// given the current state of the skeleton.
	// You will need both the bind pose world --> joint transforms.
	// and the current joint --> world transforms.
	void updateMesh();

private:

	// pointer to the root joint
	Joint* m_rootJoint;
	// the list of joints.
	std::vector< Joint* > m_joints;
	// the list of joint colors
	std::vector< FW::Vec3f > m_jointColors;

	Mesh m_mesh;

	MatrixStack m_matrixStack;
};

#endif
