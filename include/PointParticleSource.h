#ifndef _PARTICLE_SOURCE_H_
#define _PARTICLE_SOURCE_H_

#include "ParticleSource.h"

/*
 * A particle source that spawns particles at some central point.
 * Useful for things like explosions.
 */

template <class P>
class PointParticleSource : public ParticleSource {
protected:
	int x, y;
	virtual void generate_new_particles(int num);
public:
	PointParticleSource(int x, int y, int density, bool dynamic = true, bool constant = false);
	virtual ~PointParticleSource();
};

template<class P>
PointParticleSource<P>::PointParticleSource(int x, int y, int density, bool dynamic, bool constant) :
	ParticleSource(density, dynamic, constant), x{ x }, y{ x } {}

template<class P>
void PointParticleSource<P>::generate_new_particles(int num) {
	for (int x = 0; x < num; x++) {
		std::shared_ptr<P> new_particle = P::createParticleAt(x, y);
		particles.push_back(new_particle);
	}
}

#endif