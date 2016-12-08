#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "Fire.h"
#include <Particles.h>
#include <SDL_mixer.h>

class ExplosionParticle : public FireParticle {
private:
	static constexpr int explosion_radius = 100;
	static constexpr double total_time = 1.5;
	static constexpr int max_particle_radius = 6;
	static constexpr int min_particle_radius = 1;

	uint8_t start_color[8];

	double r;
	double g;
	double b;
protected:
	void fade();
public:
	ExplosionParticle(int x, int y, double angle, int radius, int dist, uint32_t color, double lifespan);
	virtual ~ExplosionParticle() {}

	static std::shared_ptr<ExplosionParticle> createParticleAt(int x, int y);
};

class Explosion : public PointParticleSource<ExplosionParticle> {
public:
	static Mix_Chunk* sound;
	Explosion(int x, int y);
	virtual bool is_over();
	static void play_sound();
};

#endif