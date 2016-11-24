#ifndef _SCATTERED_PARTICLE_SOURCE_H_
#define _SCATTERED_PARTICLE_SOURCE_H_

#include <random>
#include "ParticleSource.h"

/*
A Particle source that generates particles randomly
distributed over a rectangular area.
*/

template <class P>
class ScatteredParticleSource : public ParticleSource<P> {
protected:
	int x, y;
	unsigned int width, height;
	std::default_random_engine generator;
	std::uniform_int_distribution<int> xDist;
	std::uniform_int_distribution<int> yDist;

	// adds randomly placed particles within the specified range
	virtual void generate_new_particles(int num);
public:
	ScatteredParticleSource(int x, int y, unsigned int width, unsigned int height, int density, bool dynamic = true, bool constant = false);
	virtual ~ScatteredParticleSource() {}
};

#endif