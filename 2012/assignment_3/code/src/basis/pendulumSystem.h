#ifndef PENDULUMSYSTEM_H
#define PENDULUMSYSTEM_H

#define PI 3.14159265

#include "extra.h"
#include "base/math.hpp"
#include <vector>
#include "3rdparty/glut.h"

#include "particleSystem.h"

// Pendulum system is a string of particles connected to each other 
// through springs.
//
// p1 --- p2 --- p3 --- p4 --- ... --- pn
//
// At first you should try implementing the system with just one point connected
// to a fixed point with a spring. 
//
// You should include the spring forces, gravity and viscuous drag in your solution.
// See the handout for more details.
class PendulumSystem: public ParticleSystem
{
public:
	PendulumSystem(unsigned numParticles);
	PendulumSystem(unsigned numParticles, unsigned renderI);
    virtual ~PendulumSystem() {}
	
	vector<FW::Vec3f> evalF(vector<FW::Vec3f> state);
	
	virtual void draw();

	virtual void shiftSystem(bool forward);

protected:

	void initSystem();
	void addLink(unsigned from, unsigned to, double springConstant, double springLength);
	unsigned renderIndex;
};

#endif
