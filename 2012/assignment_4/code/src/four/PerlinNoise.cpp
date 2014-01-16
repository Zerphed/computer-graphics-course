#include "PerlinNoise.h"
#include <iostream>

// Constructor, also populates the required noise 2D array with random numbers
PerlinNoise::PerlinNoise(int octaves, float baseFrequency, float baseAmplitude, float persistance) : 
	octaves(octaves), frequency(baseFrequency), amplitude(baseAmplitude), persistance(persistance) {
	assert(octaves > 0 && baseFrequency > 0 && baseAmplitude > 0);
		
	// Initialize the 3 dimensional array with white noise
	this->noise = new float**[WIDTH];

	for (int i = 0; i < WIDTH; ++i) {
		this->noise[i] = new float*[HEIGHT];
		for (int j = 0; j < HEIGHT; ++j) {
			this->noise[i][j] = new float[DEPTH];
			for (int k = 0; k < DEPTH; ++k) {
				this->noise[i][j][k] = (rand() % 1000)/1000.0f;
			}
		}
	}
		
}

PerlinNoise::~PerlinNoise() {
		
	for (int i = 0; i < WIDTH; ++i) {
		for (int j = 0; j < HEIGHT; ++j) {
			delete [] this->noise[i][j];
		}
		delete [] this->noise[i];
	}
	delete [] this->noise;
		
}

int PerlinNoise::getOctaves() const {
	return this->octaves;
}

float PerlinNoise::getFrequency() const {
	return this->frequency;
}

float PerlinNoise::getAmplitude() const {
	return this->amplitude;
}

float PerlinNoise::getPersistance() const {
	return this->persistance;
}

float PerlinNoise::getNoise(const FW::Vec3f& point, float scale) const {

	// Call eval noise and change the scale
	float persistance = this->persistance, amp = this->amplitude;
	float s = scale, ret = 0.0f;

	for (int i = 0; i < this->octaves; i++) {
		ret += amp * this->evalNoise(point, s);
		amp *= this->persistance;
		s *= 2;
	}

	return ret/octaves;
}

// This member function is private
float PerlinNoise::evalNoise(const FW::Vec3f& point, float scale) const {

	int size = WIDTH*HEIGHT*DEPTH;

	FW::Vec3f temp = point * scale;

	float x = temp.x;
	float y = temp.y;
	float z = temp.z;

	// Step 1: get the fractional parts
	float fx = x - int(x);
	float fy = y - int(y);
	float fz = z - int(z);

	// Step 2: wrap around
	int x1 = (int(x)%WIDTH + WIDTH) % WIDTH;
	int y1 = (int(y)%HEIGHT + HEIGHT) % HEIGHT;
	int z1 = (int(z)%DEPTH + DEPTH) % DEPTH;

	// Step 3: get the neighbor values
	int x2 = (x1 + 1) % WIDTH;
	int y2 = (y1 + 1) % HEIGHT;
	int z2 = (z1 + 1) % DEPTH;

	// Step 4: smooth the random noise
	float val = 0.0f;

	val +=   fx   *   fy   * fz * this->noise[x1][y1][z1];
	val +=   fx   * (1-fy) * fz * this->noise[x1][y2][z1];
	val += (1-fx) *   fy   * fz * this->noise[x2][y1][z1];
	val += (1-fx) * (1-fy) * fz * this->noise[x2][y2][z1];

	val +=   fx   *   fy   * (1-fz) * this->noise[x1][y1][z2];
	val +=   fx   * (1-fy) * (1-fz) * this->noise[x1][y2][z2];
	val += (1-fx) *   fy   * (1-fz) * this->noise[x2][y1][z2];
	val += (1-fx) * (1-fy) * (1-fz) * this->noise[x2][y2][z2];

	return (val*0.5f)+0.5f;
}