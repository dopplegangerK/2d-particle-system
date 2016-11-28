#ifndef _LINE_PARTICLE_SOURCE_H_
#define _LINE_PARTICLE_SOURCE_H_

#include "ParticleSource.h"
#include "Vector.h"
#include <cmath>

/*
 * Generate particles randomly positioned along a line
 */

template <class P>
class LineParticleSource : public ParticleSource<P> {
protected:
	int x1, y1, x2, y2;

	virtual void generate_new_particles(int num);
public:
	LineParticleSource(int x1, int y1, int x2, int y2, int density, int dynamic = true, int constant = false) :
		ParticleSource(density, dynamic, constant), x1{ x1 }, y1{ y1 }, x2{ x2 }, y2{ y2 } {}
	virtual ~LineParticleSource();
};

template <class P>
void LineParticleSource<P>::generate_new_particles(int num) {
	for (int k = 0; k < num; k++) {
		Vector v(Point{ x1, y1 }, Point{ x2, y2 });
		double random = (rand() % (v.getLength() * 10)) / 10.0;
		Vector dist = v.scaleTo(random);
		Point p = moveBy(Point{ x1, y1 }, dist);

		std::shared_ptr<P> new_particle = P::createParticleAt(p.x, p.y);
		particles.push_back(new_particle);
	}
}

#endif