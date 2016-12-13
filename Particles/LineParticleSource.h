#ifndef _LINE_PARTICLE_SOURCE_H_
#define _LINE_PARTICLE_SOURCE_H_

#include "ParticleSource.h"
#include "Vector.h"
#include <cmath>

namespace Particles {

	/*
	 * Generate particles randomly positioned along a line
	 */

	template <class P>
	class LineParticleSource : public ParticleSource<P> {
	protected:
		Point p1, p2;

		virtual void generateNewParticles(int num);
	public:
		LineParticleSource(float x1, float y1, float x2, float y2, int density, bool dynamic = true, bool constant = false);
		LineParticleSource(Point p1, Point p2, int density, bool dynamic = true, bool constant = false);
		virtual ~LineParticleSource() {}
	};

	template<class P>
	LineParticleSource<P>::LineParticleSource(float x1, float y1, float x2, float y2, int density, bool dynamic = true, bool constant = false) :
		ParticleSource<P>(density, dynamic, constant), p1{ x1, y1 }, p2{ y1, y2 } {}

	template<class P>
	LineParticleSource<P>::LineParticleSource(Point p1, Point p2, int density, bool dynamic = true, bool constant = false) :
		LineParticleSource<P>( p1.x, p1.y, p2.x, p2.y, density, dynamic, constant ) {}

	template <class P>
	void LineParticleSource<P>::generateNewParticles(int num) {
		for (int k = 0; k < num; k++) {
			Vector v(p1, p2);

			double random = 0;
			if (v.getLength() > 0)
				random = (rand() % (int)(v.getLength() * 10)) / 10.0;
			Vector dist = v.scaleTo(random);
			Point p = moveBy(p1, dist);

			std::shared_ptr<P> new_particle = P::createParticleAt(p.x, p.y);
			ParticleSource<P>::particles.push_back(new_particle);
		}
	}
}

#endif
