#ifndef _POINT_PARTICLE_SOURCE_H_
#define _POINT_PARTICLE_SOURCE_H_

#include "ParticleSource.h"

namespace Particles {

	/*
	 * A particle source that spawns particles at some central point.
	 * Useful for things like explosions.
	 */
	template <class P>
	class PointParticleSource : public ParticleSource<P> {
	protected:
		Point p;
		virtual void generateNewParticles(int num);
	public:
		PointParticleSource(float x, float y, int density, bool dynamic = true, bool constant = false);
		PointParticleSource(Point p, int density, bool dynamic = true, bool constant = false);
		virtual ~PointParticleSource() {}
		virtual void moveTo(float x, float y);
	};

	template<class P>
	PointParticleSource<P>::PointParticleSource(float x, float y, int density, bool dynamic, bool constant) :
		ParticleSource<P>(density, dynamic, constant), p{ x, y } {}

	template<class P>
	PointParticleSource<P>::PointParticleSource(Point p, int density, bool dynamic, bool constant) :
		PointParticleSource<P>( p.x, p.y, density, dynamic, constant ) {}

	template<class P>
	void PointParticleSource<P>::generateNewParticles(int num) {
		for (int k = 0; k < num; k++) {
			std::shared_ptr<P> new_particle = P::createParticleAt(p.x, p.y);
			ParticleSource<P>::particles.push_back(new_particle);
		}
	}

	template <class P>
	void PointParticleSource<P>::moveTo(float newX, float newY) {
		p.x = newX;
		p.y = newY;
	}
}

#endif
