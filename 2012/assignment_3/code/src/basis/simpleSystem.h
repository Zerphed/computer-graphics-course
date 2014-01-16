#ifndef SIMPLESYSTEM_H
#define SIMPLESYSTEM_H

#include "extra.h"
#include "3rdparty/glut.h"
#include "base/math.hpp"
#include <vector>

#include "particleSystem.h"

using namespace std;
using namespace FW;

// TODO: SimpleSystem satisfies the differential equation
//
//    f(X, t) = (-y, x, 0), 
// 
// where X = (x, y, z). The correct trajectory for the system 
// is a circle, but the numerical solvers Euler and Trapezoidal
// will diverge from the correct trajectory into a spiral.
//
// Use this system when you are developing your integrators,
// since it is easier to see if you have a working solution.
class SimpleSystem: public ParticleSystem
{
public:
	SimpleSystem();
	
	vector<Vec3f> evalF(vector<Vec3f> state);
	
    void stepSystem(float h);

	void draw();
	
	void shiftSystem(bool forward);

    // Radius for drawing the correct trajectory! Only for visualization
    // purposes.
    float r; 

};

#endif
