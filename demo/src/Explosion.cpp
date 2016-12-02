#include "Explosion.h"
#include <SDL2_gfxPrimitives.h>
#include <cmath>

#define ORANGE 0xff5eabff
#define YELLOW 0xff44ffff
#define WHITE 0xffffffff

ExplosionParticle::ExplosionParticle(int x, int y, double angle, int radius, int dist, uint32_t c, double lifespan) :
		FireParticle(x, y, angle, radius, dist, c, lifespan) {
	r = color[0];
	g = color[1];
	b = color[2];

	*(uint32_t*)&start_color = c;
}

void ExplosionParticle::fade() {
	double amt_dead = time_lived / lifespan;
	color[3] = (uint8_t)(255 * (1 - amt_dead * amt_dead * amt_dead));

	if (b != 0) {
		b = (1 - amt_dead * amt_dead / 0.2) * start_color[0];
		if (b < 0 || amt_dead > 0.6)
			b = 0;
	}
	if (amt_dead > 0.25 && g != 0 ) {
		g = (1 - 2 * (amt_dead - 0.25) * (amt_dead - 0.25)) * start_color[1];
		if (g < 0)
			g = 0;
	}
	if (amt_dead > 0.8) {
		if (r < 0)
			r = 0;
	}
	color[0] = (int)r;
	color[1] = (int)g;
	color[2] = (int)b;
}

std::shared_ptr<ExplosionParticle> ExplosionParticle::createParticleAt(int x, int y) {
	int rad = rand() % (max_particle_radius - min_particle_radius) + min_particle_radius;

	double angle = 2 * PI * ((rand() % 200) / 200.0);

	int r = rand();
	double lifespan = (r % 100 + 100) / 100.0; //+ (r % 10) / 10.0;
	//double lifespan = 10;

	int dist = rand() % (explosion_radius /2 ) + explosion_radius/2;

	r = rand() % 4;
	uint32_t color;
	switch (r) {
	case 0:
		color = ORANGE;
		break;
	case 1:
	case 2:
		color = YELLOW;
		break;
	case 3:
		color = WHITE;
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