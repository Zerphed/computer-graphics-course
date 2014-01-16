#ifndef INTEGRATOR_H
#define INTEGRATOR_H

#include "base/math.hpp"
#include <vector>
#include "particleSystem.h"

using namespace std;

class Integrator {
public:

	Integrator(float stepSize, ParticleSystem* particleSystem) {
		h = stepSize;
		ps = particleSystem; // we will keep the api of stepSystem in ParticleSystem, instead of putting h here.
	}

	virtual ~Integrator() {
		delete ps;
	}

	virtual void stepSystem() = 0; //includes func pointer call back to evalF of psystem

	ParticleSystem* ps;
	float h;
};

#endif
