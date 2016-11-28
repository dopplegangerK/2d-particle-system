#include "Explosion.h"
#include <SDL2_gfxPrimitives.h>
#include <cmath>

#define RED 0xff002aff
#define ORANGE 0xff007fff
#define YELLOW 0xff00ffff
#define WHITE 0xffffffff

ExplosionParticle::ExplosionParticle(int x, int y, double angle, int radius, int dist, uint32_t c, double lifespan) :
		TrajectoryParticle(x, y, angle, (int)(dist / lifespan)), 
		radius{ radius }, time_lived { 0 }, lifespan{ lifespan } {
	uint32_t* color_ptr = (uint32_t*)&color[0];
	*color_ptr = c;
}

bool ExplosionParticle::is_dead() const {
	return time_lived >= lifespan;
}

void ExplosionParticle::step(double seconds) {
	TrajectoryParticle::step(seconds);
	time_lived += seconds;
}

void ExplosionParticle::draw(SDL_Renderer* ren) {
	filledCircleColor(ren, (Sint16)x, (Sint16)y, radius, *(uint32_t*)&color);
}

std::shared_ptr<ExplosionParticle> ExplosionParticle::createParticleAt(int x, int y) {
	int rad = rand() % (max_particle_radius - min_particle_radius) + min_particle_radius;

	double angle = 2 * PI * ((rand() % 200) / 200.0);

	int r = rand();
	double lifespan = (r % 75 + 75) / 100.0 + (r % 10) / 10.0;

	int dist = rand() % (explosion_radius /2 ) + explosion_radius/2;

	r = rand() % 4;
	uint32_t color;
	switch (r) {
	case 0:
		color = ORANGE;
		break;
	case 1:
		//color = WHITE;
		color = ORANGE;
		break;
	case 2:
		color = YELLOW;
		break;
	case 3:
		color = RED;
		break;
	}

	return std::make_shared<ExplosionParticle>(x, y, angle, rad, dist, color, lifespan);
}

Explosion::Explosion(int x, int y):
	PointParticleSource(x, y, 100, true, false) {
	initialize_particles();
}

bool Explosion::is_over() {
	return particles.size() == 0;
}