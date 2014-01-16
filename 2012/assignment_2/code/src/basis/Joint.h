#ifndef JOINT_H
#define JOINT_H

#include <vector>
#include <base/Math.hpp>

struct Joint
{
	FW::Mat4f transform; // transform relative to its parent
	std::vector< Joint* > children; // list of children

	// This matrix transforms world space into joint space for the initial ("bind") configuration of the joints.
	FW::Mat4f bindWorldToJointTransform;

	// This matrix maps joint space into world space for the *current* configuration of the joints.
	FW::Mat4f currentJointToWorldTransform;

	// Crufty, but best I could come up with right now, tells the idx of the joint in the joint array.
	size_t index;
};

#endif
