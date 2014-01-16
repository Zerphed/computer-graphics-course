/*
 * integratorTrapezoidal.h
 *
 *  Created on: Nov 1, 2011
 *      Author: pboyer
 */

#ifndef INTEGRATORTRAPEZOIDAL_H_
#define INTEGRATORTRAPEZOIDAL_H_

#include "base/math.hpp"
#include "integrator.h"

class TrapezoidalIntegrator: public Integrator {
public:
	TrapezoidalIntegrator(float h, ParticleSystem* ps): Integrator(h, ps) {}

	~TrapezoidalIntegrator() {}

	void stepSystem() 
    {
        // TODO: Same as with Euler. Only now you will have
        // to use the trapezoidal method for evaluating
        // new state. See handout or lecture notes for
        // information about the trapezoidal method.
		
		// Get the current state of the system, X
		std::vector<FW::Vec3f> x_0 = this->ps->getState();

		// f_0 = f(x_0, t_0)
		std::vector<FW::Vec3f> f_0 = this->ps->evalF(x_0);

		// f_1 = f(x_0 + hf_0, t_0+h)
		std::vector<FW::Vec3f> x_1(x_0.size());
		for (size_t i = 0; i < x_0.size(); i++)
			x_1[i] = x_0[i] + h*f_0[i];

		std::vector<FW::Vec3f> f_1 = this->ps->evalF(x_1);

		// x(t_0+h) = x_0 + h/2 * (f_0 + f_1)
		for (size_t i = 0; i < x_0.size(); i++)
			x_1[i] = x_0[i] + (h/2) * (f_0[i] + f_1[i]);
		
		this->ps->setState(x_1);
	}
};

#endif /* INTEGRATORTRAPEZOIDAL_H_ */
