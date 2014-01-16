/*
 * clothSystem.cpp
 *
 *  Created on: Nov 1, 2011
 *      Author: pboyer
 */

#include "extra.h"
#include "base/math.hpp"
#include "clothSystem.h"
#include <iostream>

using namespace std;
using namespace FW;



ClothSystem::ClothSystem(unsigned int rows, unsigned int columns): PendulumSystem(rows*columns) 
{
    // TODO: See the handout for how you should proceed with setting up the cloth.
    // Remember to fill the appropriate variables in ParticleSystem, which
    // this class inherits. Finally you will need to fill the faces list vecf.

    // IMPORTANT: Read the instructions in the handout for the cloth system very
    // carefully before you start implementing this function. It is easy to make
    // errors that are hard to debug.

	r = rows;
	c = columns;

	initClothSystem();
}

ClothSystem::~ClothSystem() {
	
}

void ClothSystem::initClothSystem() {
	// Physical dimensions of the cloth at initial state.
	double width = 1.5;
	double height = 1.5;

	// Default spring sizes
	double defaultSpringSizeX = width/(c-1);
	double defaultSpringSizeY = height/(r-1);
	double defaultSpringSizeShear = FW::sqrt(FW::pow(defaultSpringSizeX, 2) + FW::pow(defaultSpringSizeY, 2));
	double defaultSpringSizeFlexX = 2*defaultSpringSizeX;
	double defaultSpringSizeFlexY = 2*defaultSpringSizeY;

	double offsetX = -width/2;
	double offsetY = height/2;

	double angle = 45.0f*(PI/180.0f);
	double defaultMass = 0.025;
	double defaultSpringConstant = 20;
	double defaultDrag = 0.5;

	Vec3f defaultVelocity(0.0f, 0.0f, 0.0f);

	cout << endl;
	cout << "Current parameters:" <<endl;
	cout << "System size: " << r << ", " << c <<endl;
	cout << "Mass: " << defaultMass << endl;
	cout << "Spring Constant: " << defaultSpringConstant << endl;
	cout << "Drag constant: " << defaultDrag <<endl;

	state = vector<Vec3f>(2*numParticles);
	springDistance = vector< vector<double> >(numParticles);
	kSpring = vector< vector<double> >(numParticles);
	links = vector< vector<unsigned> >(numParticles);
	mass = vector<double>(numParticles);
	fixed = vector<bool>(numParticles); // length = numParticles
	kDrag = defaultDrag;

	for (int i = 0; i < r; i++) {
		for (int j = 0; j < c; j++) {
			int particleIndex = this->getLinearIndex(i, j);

			// Initialize state
			state[particleIndex*2] = Vec3f(offsetX+defaultSpringSizeX*j, FW::cos(angle)*(offsetY+(-defaultSpringSizeY*i)), FW::sin(angle)*(offsetY+(-defaultSpringSizeY*i)));
			state[(particleIndex*2)+1] = defaultVelocity;

			// Initialize spring related values (links, kSpring, springDistance)

			// Initialize structural springs (links)
			if (i-1 >= 0) { // Up neighbor
				this->addLink(particleIndex, this->getLinearIndex(i-1, j), defaultSpringConstant, defaultSpringSizeY);
			}
			if (i+1 < r) { // Down neighbor
				this->addLink(particleIndex, this->getLinearIndex(i+1, j), defaultSpringConstant, defaultSpringSizeY);
			}
			if (j-1 >= 0) { // Left neighbor
				this->addLink(particleIndex, this->getLinearIndex(i, j-1), defaultSpringConstant, defaultSpringSizeX);
			}
			if (j+1 < c) { // Right neighbor
				this->addLink(particleIndex, this->getLinearIndex(i, j+1), defaultSpringConstant, defaultSpringSizeX);
			}
			
			// Initialize shear springs (links)
			if (i-1 >= 0 && j-1 >= 0) { // Up left neighbor
				this->addLink(particleIndex, this->getLinearIndex(i-1, j-1), defaultSpringConstant, defaultSpringSizeShear);
			}
			if (i-1 >= 0 && j+1 < c) { // Up right neighbor
				this->addLink(particleIndex, this->getLinearIndex(i-1, j+1), defaultSpringConstant, defaultSpringSizeShear);
			}
			if (i+1 < r && j+1 < c) { // Down right neighbor
				this->addLink(particleIndex, this->getLinearIndex(i+1, j+1), defaultSpringConstant, defaultSpringSizeShear);
			}
			if (i+1 < r && j-1 >= 0) { // Down left neighbor
				this->addLink(particleIndex, this->getLinearIndex(i+1, j-1), defaultSpringConstant, defaultSpringSizeShear);
			}
			
			// Initialize flex springs (links)
			if (i-2 >= 0) { // Up flex spring
				this->addLink(particleIndex, this->getLinearIndex(i-2, j), defaultSpringConstant, defaultSpringSizeFlexY);
			}
			if (i+2 < r) { // Down flex spring
				this->addLink(particleIndex, this->getLinearIndex(i+2, j), defaultSpringConstant, defaultSpringSizeFlexY);
			}
			if (j-2 >= 0) { // Left flex spring
				this->addLink(particleIndex, this->getLinearIndex(i, j-2), defaultSpringConstant, defaultSpringSizeFlexX);
			}
			if (j+2 < c) { // Right flex spring
				this->addLink(particleIndex, this->getLinearIndex(i, j+2), defaultSpringConstant, defaultSpringSizeFlexX);
			}
			
			// Initialize mass
			mass[particleIndex] = defaultMass;

			// Initialize fixed
			if ((i == 0 && j == 0) || (i == 0 && j == c-1))
				fixed[particleIndex] = true;
			else
				fixed[particleIndex] = false;
		}
	}

	for (int i = 0; i < r-1; i++) {
		for (int j = 0; j < c-1; j++) {
			vecf.push_back(std::vector<unsigned>());
			vecf.back().push_back(getLinearIndex(i, j));
			vecf.back().push_back(getLinearIndex(i+1, j));
			vecf.back().push_back(getLinearIndex(i, j+1));

			vecf.push_back(std::vector<unsigned>());
			vecf.back().push_back(getLinearIndex(i+1, j+1));
			vecf.back().push_back(getLinearIndex(i, j+1));
			vecf.back().push_back(getLinearIndex(i+1, j));
		}
	}
}

void ClothSystem::shiftSystem(bool forward) {

	if (forward) {
		state[0].z += 0.05;
		state[2*(c-1)].z += 0.05;

	} else {
		state[0].z -= 0.05;
		state[2*(c-1)].z -= 0.05;
	}

}

// render the system (ie draw the particles)
void ClothSystem::draw()
{
	for (unsigned i = 0; i < numParticles; i++) {

		Vec3f pos = state[2*i];
		Vec3f neighbor;

		//draw lines
		/*
		glDisable(GL_LIGHTING);
		glLineWidth(2);
		glBegin(GL_LINES);
		for (unsigned j = 0; j < links[i].size(); j++) {
			neighbor = state[2*links[i][j]];
			if (i == renderIndex || links[i][j] == renderIndex)
				glColor4f(1,0,0,1);
			else
				glColor4f(1,1,1,1);
			glVertex3f(pos[0], pos[1], pos[2]);
			glVertex3f(neighbor[0], neighbor[1],neighbor[2]);
		}
		glEnd();
		*/

		glEnable(GL_LIGHTING);

		Vec3f facetNormal, v0, v1, v2;
		vector<unsigned> f;

		vector<Vec3f> normals = vector<Vec3f>(numParticles);

		for (unsigned i = 0; i < vecf.size(); i++) {
			f = vecf[i];
			v0 = state[2*f[0]];
			v1 = state[2*f[1]];
			v2 = state[2*f[2]];

			//facetNormal = Vec3f::cross(v1-v0,v2-v0);
            facetNormal = (v1-v0).cross(v2-v0);

			normals[f[0]] += facetNormal;
			normals[f[1]] += facetNormal;
			normals[f[2]] += facetNormal;
		}

		glBegin(GL_TRIANGLES);

		Vec3f n1, n2, n3;

		for (unsigned i = 0; i < vecf.size(); i++) {
			f = vecf[i];

			v0 = state[2*f[0]];
			v1 = state[2*f[1]];
			v2 = state[2*f[2]];

			n1 = normals[f[0]].normalized();
			n2 = normals[f[1]].normalized();
			n3 = normals[f[2]].normalized();

			glNormal3f(n1.x,n1.y,n1.z);
			glVertex3f(v0[0], v0[1], v0[2]);
			glNormal3f(n2.x,n2.y,n2.z);
			glVertex3f(v1[0], v1[1], v1[2]);
			glNormal3f(n3.x,n3.y,n3.z);
			glVertex3f(v2[0], v2[1], v2[2]);

		}
		glEnd();

	}
}
