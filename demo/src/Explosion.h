#ifndef _EXPLOSION_H_
#define _EXPLOSION_H_

#include "Fire.h"
#include <Particles.h>
#include <SDL_mixer.h>

using namespace Particles;

class ExplosionParticle : public FireParticle {
private:
	static constexpr int explosion_radius = 100;
	static constexpr double total_time = 1.5;
	static constexpr int max_particle_radius = 10;
	static constexpr int min_particle_radius = 3;

	uint8_t start_color[4];

	double r;
	double g;
	double b;
protected:
	void fade();
public:
	ExplosionParticle(float x, float y, double angle, int radius, int dist, uint8_t color[4], double lifespan);
	virtual ~ExplosionParticle() {}
	static std::shared_ptr<ExplosionParticle> createParticleAt(float x, float y);
};

class Explosion : public PointParticleSource<ExplosionParticle> {
public:
	static constexpr char* sound_path = "../../demo/sounds/Explosion+7.wav";
	static Mix_Chunk* sound;
	Explosion(float x, float y);
	Explosion(Point p);
	virtual bool isOver();
	static void playSound();
};

#endif
