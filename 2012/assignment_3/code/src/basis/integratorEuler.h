/*
 * integratorEuler.h
 *
 *  Created on: Nov 1, 2011
 *      Author: pboyer
 */

#ifndef INTEGRATOREULER_H_
#define INTEGRATOREULER_H_

#include "base/math.hpp"
#include "integrator.h"

class EulerIntegrator: public Integrator {
public:
	EulerIntegrator(float h, ParticleSystem* pset): Integrator(h, pset) {}

	~EulerIntegrator() {}

	void stepSystem() 
    {
        // Request the current state of the particle system ps.
		std::vector<FW::Vec3f> x_0 = this->ps->getState();
        
        // Ask the particle system to evaluate its force function (evalF)
        // for the current state.
		// You can think of this as the derivative
        std::vector<FW::Vec3f> f = this->ps->evalF(x_0);

        // Use the Euler formula to evaluate the new state, and
        // remember to set the new state for the particle system ps.
		// X(t+h) = X + hf(X,t)
		std::vector<FW::Vec3f> x_1(x_0.size());
		for (size_t i = 0; i < x_0.size(); i++)
			x_1[i] = x_0[i] + h*f[i];
		
		this->ps->setState(x_1);
	}

};

#endif /* INTEGRATOREULER_H_ */
