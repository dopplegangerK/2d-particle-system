#include "Explosion.h"
#include <SDL2_gfxPrimitives.h>
#include <cmath>

#define ORANGE {0xff, 0xab, 0x5e, 0xff}
#define YELLOW {0xff, 0xff, 0x44, 0xff} 
#define WHITE {0xff, 0xff, 0xff, 0xff}

ExplosionParticle::ExplosionParticle(float x, float y, double angle, int radius, int dist, uint8_t c[4], double lifespan) :
		FireParticle(x, y, angle, radius, dist, c, lifespan) {
	r = color[0];
	g = color[1];
	b = color[2];

	start_color[0] = c[0];
	start_color[1] = c[1];
	start_color[2] = c[2];
	start_color[3] = c[3];
}

// Fade from yellow/white to orange, then red
void ExplosionParticle::fade() {
	double amt_dead = time_lived / lifespan;
	color[3] = (uint8_t)(255 * (1 - amt_dead * amt_dead * amt_dead));

	// First reduce the blue value
	if (b != 0) {
		b = (1 - amt_dead * amt_dead / 0.2) * start_color[0]; // We'll do this quadratically because we're fancy
		if (b < 0 || amt_dead > 0.6)
			b = 0;
	}
	// After 25% of the particle's lifetime, start reducing the green value
	if (amt_dead > 0.25 && g != 0 ) {
		g = (1 - 2 * (amt_dead - 0.25) * (amt_dead - 0.25)) * start_color[1];
		if (g < 0)
			g = 0;
	}
	color[0] = (int)r;
	color[1] = (int)g;
	color[2] = (int)b;
}

std::shared_ptr<ExplosionParticle> ExplosionParticle::createParticleAt(float x, float y) {
	int rad = rand() % (max_particle_radius - min_particle_radius) + min_particle_radius;

	double angle = 2 * PI * ((rand() % 200) / 200.0);

	int r = rand();
	double lifespan = (r % 100 + 100) / 100.0;

	int dist = rand() % (explosion_radius /2 ) + explosion_radius/2;

	r = rand() % 4;

	switch (r) {
	case 0: {
		uint8_t color[4] = ORANGE;
		return std::make_shared<ExplosionParticle>(x, y, angle, rad, dist, color, lifespan);
	}
	case 1:
	case 2: {
		uint8_t color[4] = YELLOW;
		return std::make_shared<ExplosionParticle>(x, y, angle, rad, dist, color, lifespan);
	}
	default: {
		uint8_t color[4] = WHITE;
		return std::make_shared<ExplosionParticle>(x, y, angle, rad, dist, color, lifespan);
	}
	}
}

Mix_Chunk* Explosion::sound = nullptr;

Explosion::Explosion(float x, float y):
	PointParticleSource(x, y, 100, true, false) {
	initializeParticles();
}

Explosion::Explosion(Point p) : Explosion{ p.x, p.y } {}

bool Explosion::isOver() { return particles.size() == 0; }

void Explosion::playSound() { Mix_PlayChannel(-1, sound, 0); }