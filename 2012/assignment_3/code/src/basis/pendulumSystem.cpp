
#include "base/math.hpp"
#include "pendulumSystem.h"
#include <iostream>

using namespace FW;

PendulumSystem::PendulumSystem(unsigned numParticles) : ParticleSystem(numParticles)
{
	this->numParticles = numParticles;
	
	if (numParticles < 1) {
		cout << "Must specify more than 1 particle!" <<endl;
	}

    initSystem();
}

PendulumSystem::PendulumSystem(unsigned numParticles, unsigned renderI) : ParticleSystem(numParticles)  {

	renderIndex = renderI;

}

void PendulumSystem::initSystem() 
{
    // Setup the member variables of ParticleSystem which is inherited
    // by this class. See the definition of ParticleSystem (particleSystem.h)
    // for what each of the member variables mean.

    // Default values for the particle system. You may want to experiment with
    // these to see what kind of an effect they have on the dynamics of the
    // system.
	double defaultLength = 1.0;                 // Rest length of whole pendulum
	double angle = 90.0*(PI/180.0f);            // Initial angle in rad
	double defaultMass = 0.5;
	double defaultSpringConstant = 800.0;
	double defaultDrag = 1.0;

	Vec3f defaultVelocity(0.0f, 0.0f, 0.0f);

	state           = vector<Vec3f>(2*numParticles);             // x1, v1, x2, v2, ...
	springDistance  = vector< vector<double> >(numParticles);    // springDistance[i] is a list of the springDistances connected to the ith particle
	kSpring         = vector< vector<double> >(numParticles);    // Same indexing scheme
	links           = vector< vector<unsigned> >(numParticles);  // Same indexing scheme
	mass            = vector<double>(numParticles);
	fixed           = vector<bool>(numParticles);                // length = numParticles
	kDrag           = defaultDrag;

    double springLength = defaultLength/((double) numParticles-1); // Default rest lenght of a spring
	
	// Fill in code for initializing the state based on the number of particles
	for (int i = 0; i < numParticles; i++) 
    {
		// Set the mass and fixed to default values
		mass[i] = defaultMass;
		fixed[i] = false;

		// Set the state variables, i.e. position and velocity
		state[i*2] = Vec3f(FW::sin(angle)*i*springLength, FW::cos(angle)*-i*springLength, 0.0f);
		state[(i*2)+1] = defaultVelocity;

        // Populate the spring variables
		if (i == 0) {
			this->addLink(i, i+1, defaultSpringConstant, springLength);
		}
		else if (i == numParticles-1) {
			this->addLink(i, i-1, defaultSpringConstant, springLength);
		}
		else {
			this->addLink(i, i-1, defaultSpringConstant, springLength);
			this->addLink(i, i+1, defaultSpringConstant, springLength);
		}
	}

	fixed[0] = true;
}

void PendulumSystem::addLink(unsigned from, unsigned to, double springConstant, double springLength) {
	links[from].push_back(to);
	kSpring[from].push_back(springConstant);
	springDistance[from].push_back(springLength);
}

vector<Vec3f> PendulumSystem::evalF(vector<Vec3f> x)
{
	vector<Vec3f> f = vector<Vec3f>(x.size());

	Vec3f gravity = Forces::evaluateGravity();

	for (unsigned i = 0; i < (x.size()/2); i++) {
        // We are looping until (x.size() / 2) since 
        // x[2*i]   is the position of the ith particle and
        // x[2*i+1] is the velocity of the ith particle
        //
        // Remember to check if the particle i is fixed.
        //
        // The total force on the particle is the sum of gravity,
        // spring forces and viscuous drag.
        //
        // gravity: mass * constant_of_gravity
        // viscuous drag: -constant_of_viscosity * velocity of particle
        // spring: see evaluateSpring in forces.h
		if (fixed[i])
			continue;

		Vec3f windf = Vec3f(0.0f, 0.0f, 0.0f);
		if (wind)
			windf = Forces::evaluateWind();

		Vec3f drag = Forces::evaluateDrag(x[2*i+1], mass[i], kDrag);

		Vec3f spring(0.0f, 0.0f, 0.0f);
		for (size_t j = 0; j < links[i].size(); j++) {
			spring += Forces::evaluateSpring(springDistance[i][j], kSpring[i][j], mass[i], x[2*i], x[2*links[i][j]]);
		}

		f[i*2] = x[2*i+1];
		f[i*2+1] = gravity + windf + drag + spring;
    }
    
    return f;
}

// render the system (ie draw the particles)
void PendulumSystem::draw()
{
	for (unsigned i = 0; i < numParticles; i++) {

		Vec3f pos = state[2*i];
		Vec3f neighbor;

		//draw lines
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
		glEnable(GL_LIGHTING);

		glPushMatrix();
		glTranslatef(pos[0], pos[1], pos[2] );
		glutSolidSphere(0.075f,10.0f,10.0f);
		glPopMatrix();
	}
}

void PendulumSystem::shiftSystem(bool forward) {
	if (forward) {
		state[0].x -= 0.025;
	} else {
		state[0].x += 0.025;
	}
}
