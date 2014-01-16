#ifndef INTEGRATORRUNGEKUTTA_H_
#define INTEGRATORRUNGEKUTTA_H_

#include "base/math.hpp"
#include "integrator.h"

class RungeKuttaIntegrator: public Integrator {
public:
	RungeKuttaIntegrator(float h, ParticleSystem* ps): Integrator(h, ps) {
		n = ps->getState().size();

		t1 = std::vector<FW::Vec3f>(n);
		t2 = std::vector<FW::Vec3f>(n);
		t3 = std::vector<FW::Vec3f>(n);

		k1 = std::vector<FW::Vec3f>(n);
		k2 = std::vector<FW::Vec3f>(n);
		k3 = std::vector<FW::Vec3f>(n);
		k4 = std::vector<FW::Vec3f>(n);
	}

	~RungeKuttaIntegrator() {}

	void stepSystem() 
    {
		// Evaluate the new state using Runge-Kutta 4:
		//
		// y_n+1 = y_n + 1/6(k_1 + 2k_2 + 2k_3 + k_4), where y_n+1 is the RK4
		// approximation of y(t_n+1), and
		//
		// Step 1: k_1 = hf(t_n, y_n),
		// Step 2: k_2 = hf(t_n + (1/2)h, y_n + (1/2)k_1)
		// Step 3: k_3 = hf(t_n + (1/2)h, y_n + (1/2)k_2)
		// Step 4: k_4 = hf(t_n + h, y_n + k_3)
		//
		// Step 5: Profit
		// y_n+1 = y_n + 1/6(k_1 + 2k_2 + 2k_3 + k_4)
		std::vector<FW::Vec3f> y  = this->ps->getState();
		std::vector<FW::Vec3f> d0 = this->ps->evalF(y);
		
		size_t i;
		float step = h/2.0f;

		// Step 1
		for (i=0; i < n; i++)
			t1[i] = y[i] + 0.5f * (k1[i] = step * d0[i]);
		
		// Step 2
		std::vector<FW::Vec3f> d1 = this->ps->evalF(t1);
		for (i=0; i < n; i++)
			t2[i] = y[i] + 0.5f * (k2[i] = step * d1[i]);

		// Step 3
		std::vector<FW::Vec3f> d2 = this->ps->evalF(t2);
		for (i=0; i < n; i++)
			t3[i] = y[i] + (k3[i] = step * d2[i]);

		// Step 4
		std::vector<FW::Vec3f> d3 = this->ps->evalF(t3);
		for (i=0; i < n; i++)
			k4[i] = h * d3[i];

		// Step 5
		for (i=0; i < n; i++)
			y[i] += (k1[i] + 2.0f*k2[i] + 2.0f*k3[i] + k4[i])/6.0f;

		ps->setState(y);
	}

private:
	size_t n;

	std::vector<FW::Vec3f> t1;
	std::vector<FW::Vec3f> t2;
	std::vector<FW::Vec3f> t3;

	std::vector<FW::Vec3f> k1;
	std::vector<FW::Vec3f> k2;
	std::vector<FW::Vec3f> k3;
	std::vector<FW::Vec3f> k4;
};

#endif /* INTEGRATORRUNGEKUTTA_H_ */