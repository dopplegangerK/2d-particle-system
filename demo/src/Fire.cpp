#include "Fire.h"

#define WHITE 0xffffffff

int FireParticle::height = 0;
int FireParticle::width = 0;
SDL_Texture* FireParticle::tex = nullptr;

FireParticle::FireParticle(int x, int y, double angle, int radius, int dist, uint32_t c, double lifespan):
		TrajectoryParticle(x, y, angle, (int)(dist/lifespan)),
		radius{ radius }, time_lived{ 0 }, lifespan{ lifespan } {
	uint32_t* color_ptr = (uint32_t*)&color[0];
	*color_ptr = c;
	alpha = color[3];

        rect.w = radius * 2;
        rect.h = radius * 2;
}

bool FireParticle::is_dead() const {
	return time_lived >= lifespan;
}

void FireParticle::step(double seconds) {
	TrajectoryParticle::step(seconds);
	time_lived += seconds;
	fade();
}

void FireParticle::fade() {
	color[3] = (uint8_t)(alpha * (1 - time_lived / lifespan));
}

void FireParticle::draw(SDL_Renderer* ren) {
	//filledCircleColor(ren, (Sint16)x, (Sint16)y, radius, *(uint32_t*)&color);
	
	//rachet halo particles
	/*
	uint8_t alpha = color[3];
	color[3] = alpha / 4;
	filledCircleColor(ren, (Sint16)x, (Sint16)y, (int)(radius * 1.2), *(uint32_t*)&color);
	filledCircleColor(ren, (Sint16)x, (Sint16)y, (int)(radius), *(uint32_t*)&color);
	color[3] = alpha;
	filledCircleColor(ren, (Sint16)x, (Sint16)y, radius * 0.5, *(uint32_t*)&color);
	*/

	//racheter halo particles
/*	
	uint8_t alpha = color[3];
	color[3] = alpha / 4;
	filledCircleColor(ren, (Sint16)x, (Sint16)y, (int)(radius + 2), *(uint32_t*)&color);
	color[3] = alpha;
	filledCircleColor(ren, (Sint16)x, (Sint16)y, radius, *(uint32_t*)&color);
        */

        //less rachet halo particles
        rect.x = x - radius;
        rect.y = y - radius;        
        SDL_SetTextureColorMod(tex, color[0], color[1], color[2]);
        SDL_SetTextureAlphaMod(tex, color[3]);
	SDL_RenderCopy(ren, tex, NULL, &rect);
}

/* ROCKET FIRE PARTICLE */

double RocketFireParticle::spawn_angle = 0;


RocketFireParticle::RocketFireParticle(int x, int y, double angle, int radius, int dist, uint32_t color, double lifespan) :
	FireParticle(x, y, angle, radius, dist, color, lifespan) {
	r = this->color[0];
	g = this->color[1];
	b = this->color[2];

	*(uint32_t*)&start_color = color;
}

void RocketFireParticle::set_angle(double angle) { spawn_angle = angle; }

void RocketFireParticle::fade() {
	
	//FireParticle::fade();
	double amt_dead = time_lived / lifespan;

	color[0] = (uint8_t)(255 * (1 - amt_dead));
	color[1] = color[0];
	
	/*
	double amt_dead = time_lived / lifespan;
	color[3] = (uint8_t)(255 * (1 - amt_dead * amt_dead * amt_dead));

	if (b != 0) {
		b = (1 - amt_dead * amt_dead / 0.2) * start_color[0];
		if (b < 0 || amt_dead > 0.6)
			b = 0;
	}
	if (amt_dead > 0.25 && g != 0) {
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
	*/
}

std::shared_ptr<RocketFireParticle> RocketFireParticle::createParticleAt(int x, int y) {
	int rad = rand() % (max_particle_radius - min_particle_radius) + min_particle_radius;

	int r = rand();
	double lifespan = ((r % 200) / 200.0) * max_time;

	int dist = rand() % max_dist;

	return std::make_shared<RocketFireParticle>(x, y, spawn_angle, rad, dist, WHITE, lifespan);
}

RocketFireSource::RocketFireSource() : RocketFireSource({ 0, 0 }, { 0, 0 }) {}

RocketFireSource::RocketFireSource(Point a, Point b) : LineParticleSource(a.x, a.y, b.x, b.y, 10, true, true) {
	initialize_particles();
}

void RocketFireSource::moveTo(Point newA, Point newB) {
	x1 = newA.x;
	y1 = newA.y;
	x2 = newB.x;
	y2 = newB.y;
}

void RocketFireSource::generate_new_particles(int num) {
	RocketFireParticle::set_angle(Vector({ x1, y1 }, { x2, y2 }).getPerp().getAngle());
	LineParticleSource::generate_new_particles(num);
}
