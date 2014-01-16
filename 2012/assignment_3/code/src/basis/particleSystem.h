#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <vector>
#include "base/math.hpp"
#include "forces.h"

using namespace std;
using namespace FW;

enum ParticleSystemType
{
    ParticleSystemType_Simple = 0,
    ParticleSystemType_Pendulum,
    ParticleSystemType_Cloth
};

class ParticleSystem
{
public:

	// YOUR CONSTRUCTOR
	ParticleSystem(int numParticles); // assigns default integrator
    virtual ~ParticleSystem() {}

	int numParticles;
	
	// for a given state, evaluate f(X,t)
	virtual vector<Vec3f> evalF(vector<Vec3f> state) = 0;
	
	// getter method for the system's state
	vector<Vec3f> getState();
	
	// setter method for the system's state
	void setState(vector<Vec3f> newState);

	enum ParticleSystemType getSystemType();
	void toggleWind();

	virtual void draw() = 0;
	
	virtual void shiftSystem(bool forward) = 0;

protected:

	vector<Vec3f> state; //length 2*numParticles   x, v, x, v, ...

    // These three variables have identical dimensions. For example links[i] contains
    // the indices of the particles that are connected to the ith particle through
    // a spring.
	vector< vector<double> > springDistance;  //length numParticles with each vector different
	vector< vector<double> > kSpring;
	vector< vector<unsigned int> > links;

	vector<double> mass; // length = numParticles
	vector<bool> fixed; // length = numParticles
	double kDrag;

	// For wind emulation with cloth
	bool wind;
};

#endif
