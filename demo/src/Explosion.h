#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "Fire.h"
#include <Particles.h>

class ExplosionParticle : public FireParticle {
private:
	static constexpr int explosion_radius = 100;
	static constexpr double total_time = 1.0;
	static constexpr int max_particle_radius = 6;
	static constexpr int min_particle_radius = 1;
public:
	ExplosionParticle(int x, int y, double angle, int radius, int dist, uint32_t color, double lifespan);
	virtual ~ExplosionParticle() {}

	static std::shared_ptr<ExplosionParticle> createParticleAt(int x, int y);
};

class Explosion : public PointParticleSource<ExplosionParticle> {
public:
	Explosion(int x, int y);
	virtual bool is_over();
};

#endif