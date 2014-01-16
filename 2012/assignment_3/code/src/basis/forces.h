/*
 * forces.h
 *
 *  Created on: Nov 1, 2011
 *      Author: pboyer
 */

#ifndef FORCES_H_
#define FORCES_H_

#include "extra.h"
#include <vector>
#include <random>

using namespace std;
using namespace FW;

class Forces {

public:

    // Gravity divided by mass.
	static Vec3f evaluateGravity() 
    {
		return Vec3f(0.0f,-9.8f,0.0f);
	}

	static Vec3f evaluateDrag(Vec3f velocity, float mass, float dragConstant) 
    {
        // Evaluate drag force on a particle. You should divide the 
        // actual force with the mass to get massless units. The integrators
        // expect such forces.
        return (-dragConstant*velocity)/mass;
	}

	static Vec3f evaluateSpring(float dist0, float k, float nodeMass, Vec3f node, Vec3f neighbor) 
    {
        // Evaluate spring force divided by mass. The integrators expect massless units.
		Vec3f d = node-neighbor;
		return (-k*(d.length()-dist0)*(d/d.length()))/nodeMass;
	}

	static Vec3f evaluateWind() {
		static std::default_random_engine generator;
		static std::normal_distribution<double> distribution_x(5.0, 45.0);
		static std::normal_distribution<double> distribution_z(15.0, 100.0);

		return FW::Vec3f(distribution_x(generator), 0.0f, distribution_z(generator));
	}
};



#endif /* FORCES_H_ */
