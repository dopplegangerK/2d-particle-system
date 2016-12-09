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


template <class P>
ScatteredParticleSource<P>::ScatteredParticleSource(int x, int y, unsigned int w, unsigned int h, int density, bool dynamic, bool constant) :
	ParticleSource<P>(density, dynamic, constant), x{ x }, y{ x }, width{ w }, height{ h }, xDist(x, x + w), yDist(y, y + h) { }

template <class P>
void ScatteredParticleSource<P>::generate_new_particles(int num) {
	//make sure the area in which to generate particles hasn't changed (in case the entire block was moved)
	if (xDist.min() != x || xDist.max() != x + width)
		xDist = std::uniform_int_distribution<int>(x, x + width);
	if (yDist.min() != y || yDist.max() != y + height)
		yDist = std::uniform_int_distribution<int>(y, y + height);

	for (int k = 0; k < num; k++) {
		int myX = xDist(generator);
		int myY = yDist(generator);
		std::shared_ptr<P> new_particle = P::createParticleAt(myX, myY);
                ParticleSource<P>::particles.push_back(new_particle);
	}
}


#endif
