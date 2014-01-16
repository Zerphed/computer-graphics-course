#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include <cassert>
#include <cstdlib>
#include "base/math.hpp"

#define WIDTH 192
#define HEIGHT 192
#define DEPTH 192

class PerlinNoise
{

public:

	// Constructor, also populates the required noise 2D array with random numbers
	PerlinNoise(int octaves, float baseFrequency = 1.0f, float baseAmplitude = 1.0f, float persistance = 0.5f);

	~PerlinNoise();

	int getOctaves() const;
	float getFrequency() const;
	float getAmplitude() const;
	float getPersistance() const;

	float getNoise(const FW::Vec3f& point, float scale) const;
	

private:

	float evalNoise(const FW::Vec3f& point, float scale) const;

	int octaves;
	float frequency, amplitude, persistance;
    float*** noise;
};

#endif // PERLINNOISE_H