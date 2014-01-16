#include "base/math.hpp"
#include "particleSystem.h"

ParticleSystem::ParticleSystem(int numPart) {
	numParticles = numPart;
	wind = false;
}

vector<FW::Vec3f> ParticleSystem::getState() {
	return state;
}

void ParticleSystem::setState(vector<FW::Vec3f> input) {
	state = input;
}

void ParticleSystem::toggleWind() {
	wind = !wind;
}
