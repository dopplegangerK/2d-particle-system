#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "Particle.h"
#include "PointParticleSource.h"

class ExplosionParticle : public TrajectoryParticle {
private:
	static constexpr int explosion_radius = 100;
	static constexpr double total_time = 1.0;
	static constexpr int max_particle_radius = 6;
	static constexpr int min_particle_radius = 1;

	int radius;
	double lifespan;
	double time_lived;
	uint8_t color[4];
public:
	ExplosionParticle(int x, int y, double angle, int radius, int dist, uint32_t color, double lifespan);
	virtual ~ExplosionParticle() {}
	virtual bool is_dead() const;
	virtual void step(double seconds);
	virtual void draw(SDL_Renderer* ren);

	static std::shared_ptr<ExplosionParticle> createParticleAt(int x, int y);
};

class Explosion : public PointParticleSource<ExplosionParticle> {
public:
	Explosion(int x, int y);
	virtual bool is_over();
};

#endif