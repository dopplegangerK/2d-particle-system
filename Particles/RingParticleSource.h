#ifndef _RING_PARTICLE_SOURCE_H_
#define _RING_PARTICLE_SOURCE_H_

#include "ParticleSource.h"
#include "Vector.h"
#include <cmath>
#include <iostream>
#include <SDL2_gfxPrimitives.h>

namespace Particles {

	/*
	 * Generate particles randomly placed along the perimiter of a circle
	 */

	template <class P>
	class RingParticleSource : public ParticleSource<P> {
	protected:
		Point center;
		float radius;

		virtual void generateNewParticles(int num);
	public:
		RingParticleSource(float x, float y, float radius, int density, bool dynamic = true, bool constant = false);
		RingParticleSource(Point center, float radius, int density, bool dynamic = true, bool constant = false);

		virtual ~RingParticleSource() {}
	};

	template<class P>
	RingParticleSource<P>::RingParticleSource(float x, float y, float radius, int density, bool dynamic = true, bool constant = false):
		ParticleSource<P>(density, dynamic, constant), center{ x, y }, radius{ radius } {}

	template<class P>
	RingParticleSource<P>::RingParticleSource(Point c, float radius, int density, bool dynamic = true, bool constant = false) :
		RingParticleSource<P>( c.x, c.y, radius, density, dynamic, constant ) {}

	template <class P>
	void RingParticleSource<P>::generateNewParticles(int num) {
		for (int k = 0; k < num; k++) {
			Point p = getPointOnRing(center, radius);
			std::shared_ptr<P> new_particle = P::createParticleAt(p.x, p.y);
			ParticleSource<P>::particles.push_back(new_particle);
		}
	}
}

#endif
