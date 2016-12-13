#ifndef _SCATTERED_PARTICLE_SOURCE_H_
#define _SCATTERED_PARTICLE_SOURCE_H_

#include <random>
#include "ParticleSource.h"

namespace Particles {

	/*
	A Particle source that generates particles randomly
	distributed over a rectangular area.
	*/

	template <class P>
	class ScatteredParticleSource : public ParticleSource<P> {
	protected:
		Point upper_left;
		unsigned int width, height;
		std::default_random_engine generator;
		std::uniform_int_distribution<int> xDist;
		std::uniform_int_distribution<int> yDist;

		// adds randomly placed particles within the specified range
		virtual void generateNewParticles(int num);
	public:
		ScatteredParticleSource(int x, int y, unsigned int width, unsigned int height, int density, bool dynamic = true, bool constant = false);
		ScatteredParticleSource(Point corner, unsigned int width, unsigned int height, int density, bool dynamic = true, bool constant = false);
		virtual ~ScatteredParticleSource() {}
	};


	template <class P>
	ScatteredParticleSource<P>::ScatteredParticleSource(int x, int y, unsigned int w, unsigned int h, int density, bool dynamic, bool constant) :
		ParticleSource<P>(density, dynamic, constant), upper_left{ (float)x, (float)y }, width{ w }, height{ h }, xDist(x, x + w), yDist(y, y + h) { }

	template <class P>
	ScatteredParticleSource<P>::ScatteredParticleSource(Point p, unsigned int w, unsigned int h, int density, bool dynamic, bool constant) :
		ScatteredParticleSource<P>( (int)p.x, (int)p.y, w, h, density, dynamic, constant ) { }

	template <class P>
	void ScatteredParticleSource<P>::generateNewParticles(int num) {
		//make sure the area in which to generate particles hasn't changed (in case the entire block was moved)
		if (xDist.min() != upper_left.x || xDist.max() != upper_left.x + width)
			xDist = std::uniform_int_distribution<int>((int)upper_left.x, (int)upper_left.x + width);
		if (yDist.min() != upper_left.y || yDist.max() != upper_left.y + height)
			yDist = std::uniform_int_distribution<int>((int)upper_left.y, (int)upper_left.y + height);

		for (int k = 0; k < num; k++) {
			float myX = (float)xDist(generator);
			float myY = (float)yDist(generator);
			std::shared_ptr<P> new_particle = P::createParticleAt(myX, myY);
			ParticleSource<P>::particles.push_back(new_particle);
		}
	}
}

#endif
