#include "SkeletalModel.h"
#include "3rdparty/glut.h"
#include "extra.h"

//#include <FL/Fl.H>

#define PI 3.14159265

using namespace std;

SkeletalModel::~SkeletalModel (void)
{
    for (unsigned i = 0; i < m_joints.size(); ++i)
        delete m_joints[i];
}

void SkeletalModel::load(const char *skeletonFile, const char *meshFile, const char *attachmentsFile)
{
	loadSkeleton(skeletonFile);

	m_mesh.load(meshFile);
	m_mesh.loadAttachments(attachmentsFile, m_joints.size());

	// Populate joint indices
	for (size_t i = 0; i < m_joints.size(); i++)
		m_joints[i]->index = i;

	// Calculate the per vertex colors of the mesh
	computeJointColors();
	computeVertexColors();

	computeBindWorldToJointTransforms();
	updateCurrentJointToWorldTransforms();
}

void SkeletalModel::draw(FW::Mat4f cameraMatrix, bool skeletonVisible)
{
	// draw() gets called whenever a redraw is required
	// (after an update() occurs, when the camera moves, the window is resized, etc)

	m_matrixStack.clear();
	m_matrixStack.push(cameraMatrix);

	if( skeletonVisible )
	{
		drawJoints();

		drawSkeleton();
	}
	else
	{
		// Clear out any weird matrix we may have been using for drawing the bones and revert to the camera matrix.
		glLoadMatrixf(m_matrixStack.top().getPtr());

		// Tell the mesh to draw itself.
		m_mesh.draw();
	}
}

void SkeletalModel::loadSkeleton( const char* filename )
{
	ifstream filestream ( filename );

	while(filestream.good())
	{
		Joint *node = new Joint();

		float x,y,z;
		filestream >> x >> y >> z;		
		node->transform.setIdentity ();
		FW::Vec4f lastColumn (x, y, z, 1);
		node->transform.setCol (3, lastColumn);

		if(filestream.good() == false)
		{
			delete node;
			break;
		}

		int parentIndex;
		filestream >> parentIndex;

		m_joints.push_back(node);

		if(parentIndex == -1)
		{
			m_rootJoint = node;
		}
		else
		{
			m_joints[parentIndex]->children.push_back(node);
		}
	}

	filestream.close();

}

void SkeletalModel::drawJoints( )
{
	// Draw a sphere at each joint. You will need to use the recursive helper function 
    // drawJointsRecurse to traverse the joint hierarchy.
	//
	// We recommend using glutSolidSphere( 0.025f, 12, 12 )
	// to draw a sphere of reasonable size.
	//
	// You are *not* permitted to use the OpenGL matrix stack commands
	// (glPushMatrix, glPopMatrix, glMultMatrix).
	// You should use your MatrixStack class
	// and use glLoadMatrix() before your drawing call.

	// Call the recursive drawing function
	drawJointsRecurse(m_rootJoint);
}


void SkeletalModel::drawJointsRecurse( Joint* joint )
{
    // Draw current joint. Recursively draw all children of current joint.
    // You can think of the recursion as a depth first traversal of the
    // joint graph.

	// 1. Push new transform
	m_matrixStack.push(joint->transform);

	// 2. Draw current
	glLoadMatrixf(m_matrixStack.top().getPtr());

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, m_jointColors[joint->index].getPtr());
	glutSolidSphere(0.025f, 12, 12);

	// 3. For each child of joint
	//    3.1 recurse
	//    3.2 pop stack
	for (std::vector<Joint*>::iterator it = joint->children.begin(); it != joint->children.end(); it++)
		drawJointsRecurse(*it);
	m_matrixStack.pop();
}


void SkeletalModel::drawSkeleton( )
{
	// Draw boxes between the joints. You will need to use the recursive helper function 
    // drawSkeletonRecurse to traverse the joint hierarchy.
	drawSkeletonRecurse(m_rootJoint);
}


void SkeletalModel::drawSkeletonRecurse(Joint *joint)
{
	m_matrixStack.push(joint->transform);
	glLoadMatrixf(m_matrixStack.top().getPtr());

	for (std::vector<Joint*>::iterator it = joint->children.begin(); it != joint->children.end(); it++) {
		// Initial transform starts from identity
		FW::Mat4f transform;
		transform.setIdentity();

		// Translate in z: box must range from [-0.5, -0.5, 0] to [0.5, 0.5. 1]
		FW::Mat4f T = translation(FW::Vec3f(0.0f, 0.0f, 0.5f));

		// Scale the box so that it ranges from [-0.05, -0.05, 0] to [0.05, 0.05, length]
		FW::Vec3f childTranslation = FW::Vec3f((*it)->transform.m03, (*it)->transform.m13, (*it)->transform.m23);
		float length = childTranslation.length();

		FW::Mat4f S;
		S.setIdentity();
		S.m00 = S.m11 = 0.05f; // Note: only to make things match with the example, if I was following
		S.m22 = length;        //       the instructions, this should be 0.1.

		// Rotate
		FW::Vec3f rnd(0.0f, 0.0f, 1.0f);
		FW::Vec3f zNew = childTranslation.normalized();
		FW::Vec3f yNew = zNew.cross(rnd).normalized();
		FW::Vec3f xNew = yNew.cross(zNew).normalized();

		//DrawDebugLine(zNew.x, zNew.y, zNew.z, 0, 0, 0);

		FW::Mat4f R = makeMat4f(xNew.x, yNew.x, zNew.x, 0.0f,
							    xNew.y, yNew.y, zNew.y, 0.0f,
							    xNew.z, yNew.z, zNew.z, 0.0f,
								 0.0f,   0.0f,   0.0f,  1.0f);

		// Add all the transforms
		transform *= R;
		transform *= S;
		transform *= T;

		m_matrixStack.push(transform);	// Push
		glLoadMatrixf(m_matrixStack.top().getPtr());
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, FW::Vec3f(0.35f, 0.35f, 0.35f).getPtr());
		glutSolidCube(1.0f);			// Draw
		m_matrixStack.pop();			// Pop

		// Recurse
		drawSkeletonRecurse(*it);
	}
	m_matrixStack.pop();
}


void SkeletalModel::setJointTransform(int jointIndex, float rX, float rY, float rZ)
{
	// Set the rotation part of the joint's transformation matrix based on the passed in Euler angles.
    // The rotation can be written as
    // 
    //     R = Rz(rZ) * Ry(rY) * Rx(rX)
    //
    // where Rx(theta) is a rotation of angle theta around the x axis.
    // FW::Mat3f has a function for constructing rotations given an angle and an axis.

	// Calculate the individual transformations on x,y and z axes
	FW::Mat3f rotationX = FW::Mat3f::rotation(FW::Vec3f(1.0f, 0.0f, 0.0f), rX);
	FW::Mat3f rotationY = FW::Mat3f::rotation(FW::Vec3f(0.0f, 1.0f, 0.0f), rY);
	FW::Mat3f rotationZ = FW::Mat3f::rotation(FW::Vec3f(0.0f, 0.0f, 1.0f), rZ);

	// Compute the final rotation matrix by combining the individual rotations
	FW::Mat3f R = rotationZ * rotationY * rotationX;
	
	// Set the values to all cells except the cells responsible for translation
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			this->m_joints[jointIndex]->transform.set(i, j, R.get(i,j));
}


void SkeletalModel::computeBindWorldToJointTransforms()
{
	// 2.3.1. Implement this method to compute a per-joint transform from
	// world-space to joint space in the BIND POSE.
	//
	// Note that this needs to be computed only once since there is only
	// a single bind pose.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
	FW::Mat4f identity;
	identity.setIdentity();
	computeBindWorldToJointTransformsRecurse(m_rootJoint, identity);
}


void SkeletalModel::computeJointColors() {
	// Populate the joint colors (works for up to 36 joints or so)
	for (size_t i = 0; i < m_joints.size(); i++) {
		m_jointColors.push_back( FW::Vec3f( 0.25f*((i/9)+1), 0.25f*(((i%9)/3)+1), 0.25f*(((i%9)%4)+1) ) );
	}
}

void SkeletalModel::computeVertexColors() {
	for (size_t i = 0; i < m_mesh.bindVertices.size(); i++) {
		m_mesh.vertexColors.push_back(FW::Vec3f(0.0f, 0.0f, 0.0f));
		for (size_t j = 0; j < m_joints.size(); j++) {
			if (m_mesh.attachments[i][j] > 0.0f)
				m_mesh.vertexColors[i] += m_mesh.attachments[i][j] * m_jointColors[j];
		}
	}
}

void SkeletalModel::computeBindWorldToJointTransformsRecurse(Joint *joint, FW::Mat4f currentMatrix)
{
	// Use currentMatrix and joint->transform to update the binding transform
	joint->bindWorldToJointTransform = currentMatrix * joint->transform.inverted();
	for (std::vector<Joint*>::iterator it = joint->children.begin(); it != joint->children.end(); it++) {
		computeBindWorldToJointTransformsRecurse(*it, joint->bindWorldToJointTransform);
	}
}


void SkeletalModel::updateCurrentJointToWorldTransforms()
{
	// 2.3.2. Implement this method to compute a per-joint transform from
	// joint space to world space in the CURRENT POSE.
	//
	// The current pose is defined by the rotations you've applied to the
	// joints and hence needs to be *updated* every time the joint angles change.
	//
	// This method should update each joint's bindWorldToJointTransform.
	// You will need to add a recursive helper function to traverse the joint hierarchy.
	FW::Mat4f identity;
	identity.setIdentity();
	updateCurrentJointToWorldTransformsRecurse(m_rootJoint, identity);
}


void SkeletalModel::updateCurrentJointToWorldTransformsRecurse(Joint *joint, FW::Mat4f currentMatrix)
{
    // Use currentMatrix and joint->transform to update current joint to world transform.
    // Call recursively for all children.
	// Use currentMatrix and joint->transform to update the binding transform
	joint->currentJointToWorldTransform = currentMatrix * joint->transform;
	for (std::vector<Joint*>::iterator it = joint->children.begin(); it != joint->children.end(); it++) {
		updateCurrentJointToWorldTransformsRecurse(*it, joint->currentJointToWorldTransform);
	}
}


void SkeletalModel::updateMesh()
{
	// 2.3.2. This is the core of SSD.
	// Implement this method to update the vertices of the mesh
	// given the current state of the skeleton.
	// You will need both the bind pose world --> joint transforms.
	// and the current joint --> world transforms. You will need to use
    // the attachment weights here.

	size_t meshBindVerticesSize = m_mesh.bindVertices.size();
	size_t jointsSize = m_joints.size();
	FW::Vec4f p_new(0.0f, 0.0f, 0.0f, 0.0f);

	// p'_i = SUM(W_ij * T_j(t) * B_j^-1 * p_i)
	//         j
	for (size_t i = 0; i < meshBindVerticesSize; i++) {
		p_new.setZero();
		for (size_t j = 0; j < jointsSize; j++) {
			if (m_mesh.attachments[i][j] > 0.0f)
				p_new += m_mesh.attachments[i][j] * (m_joints[j]->currentJointToWorldTransform * m_joints[j]->bindWorldToJointTransform 
				* FW::Vec4f(m_mesh.bindVertices[i].x, m_mesh.bindVertices[i].y, m_mesh.bindVertices[i].z, 1.0f));
		}
		m_mesh.currentVertices[i] = p_new.getXYZ();
	}
}

