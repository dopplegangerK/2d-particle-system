#include "Fire.h"

#define WHITE 0xffffffff

int FireParticle::height = 0;
int FireParticle::width = 0;
SDL_Texture* FireParticle::tex = nullptr;

FireParticle::FireParticle(float x, float y, double angle, int radius, int dist, uint8_t c[4], double lifespan):
		TrajectoryParticle(x, y, angle, (int)(dist/lifespan)),
		radius{ radius }, time_lived{ 0 }, lifespan{ lifespan } {
	color[0] = c[0];
	color[1] = c[1];
	color[2] = c[2];
	color[3] = c[3];

	alpha = color[3];
	
	rect.w = radius * 2;
	rect.h = radius * 2;
}

bool FireParticle::isDead() const {
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
	//less rachet halo particles
	rect.x = (int)p.x - radius;
	rect.y = (int)p.y - radius;
	SDL_SetTextureColorMod(tex, color[0], color[1], color[2]);
	SDL_SetTextureAlphaMod(tex, color[3]);
	SDL_RenderCopy(ren, tex, NULL, &rect);
}

/* ROCKET FIRE PARTICLE */

double RocketFireParticle::spawn_angle = 0;

RocketFireParticle::RocketFireParticle(float x, float y, double angle, int radius, int dist, uint8_t color[4], double lifespan) :
	FireParticle(x, y, angle, radius, dist, color, lifespan) {
	r = this->color[0];
	g = this->color[1];
	b = this->color[2];

	start_color[0] = color[0];
	start_color[1] = color[1];
	start_color[2] = color[2];
	start_color[3] = color[3];
}

void RocketFireParticle::setAngle(double angle) { spawn_angle = angle; }

void RocketFireParticle::fade() {
	// Just make it more blue
	double amt_dead = time_lived / lifespan;

	color[0] = (uint8_t)(255 * (1 - amt_dead));
	color[1] = color[0];
}

std::shared_ptr<RocketFireParticle> RocketFireParticle::createParticleAt(float x, float y) {
	int rad = rand() % (max_particle_radius - min_particle_radius) + min_particle_radius;

	int r = rand();
	double lifespan = ((r % 200) / 200.0) * max_time;

	int dist = rand() % max_dist;

	uint8_t white[4] = { 255, 255, 255, 255 };
	return std::make_shared<RocketFireParticle>(x, y, spawn_angle, rad, dist, white, lifespan);
}

RocketFireSource::RocketFireSource() : RocketFireSource({ 0, 0 }, { 0, 0 }) {}

RocketFireSource::RocketFireSource(Point a, Point b) : LineParticleSource(a.x, a.y, b.x, b.y, 10, true, true) {
	initializeParticles();
}

void RocketFireSource::moveTo(Point newA, Point newB) {
	p1 = newA;
	p2 = newB;
}

void RocketFireSource::generateNewParticles(int num) {
	RocketFireParticle::setAngle(Vector(p1, p2).getPerp().getAngle());
	LineParticleSource::generateNewParticles(num);
}
