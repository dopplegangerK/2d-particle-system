#ifndef _RING_PARTICLE_SOURCE_H_
#define _RING_PARTICLE_SOURCE_H_

#include "ParticleSource.h"
#include "Vector.h"
#include <cmath>
#include <iostream>
#include <SDL2_gfxPrimitives.h>

/*
 * Generate particles randomly placed along the perimiter of a circle
 */

template <class P>
class RingParticleSource : public ParticleSource<P> {
protected:
	int x, y;
	int radius;

	virtual void generate_new_particles(int num);
public:
	RingParticleSource(int x, int y, int radius, int density, bool dynamic = true, bool constant = false) :
		ParticleSource(density, dynamic, constant), x{ x }, y{ y }, radius{ radius } {}
	virtual ~RingParticleSource() {}
};

template <class P>
void RingParticleSource<P>::generate_new_particles(int num) {
	for (int k = 0; k < num; k++) {
		Point p = getPointOnRing({ x, y }, radius);
		std::shared_ptr<P> new_particle = P::createParticleAt(p.x, p.y);
		particles.push_back(new_particle);
	}
}

#endif