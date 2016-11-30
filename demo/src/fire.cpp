#include "fire.h"

#define WHITE 0xffffffff

FireParticle::FireParticle(int x, int y, double angle, int radius, int dist, uint32_t c, double lifespan):
		TrajectoryParticle(x, y, angle, (int)(dist/lifespan)),
		radius{ radius }, time_lived{ 0 }, lifespan{ lifespan } {
	uint32_t* color_ptr = (uint32_t*)&color[0];
	*color_ptr = c;
}

bool FireParticle::is_dead() const {
	return time_lived >= lifespan;
}

void FireParticle::step(double seconds) {
	TrajectoryParticle::step(seconds);
	time_lived += seconds;
}

void FireParticle::draw(SDL_Renderer* ren) {
	filledCircleColor(ren, (Sint16)x, (Sint16)y, radius, *(uint32_t*)&color);
}

/* ROCKET FIRE PARTICLE */

double RocketFireParticle::spawn_angle = 0;


RocketFireParticle::RocketFireParticle(int x, int y, double angle, int radius, int dist, uint32_t color, double lifespan) :
	FireParticle(x, y, angle, radius, dist, color, lifespan) {}

void RocketFireParticle::set_angle(double angle) { spawn_angle = angle; }

std::shared_ptr<RocketFireParticle> RocketFireParticle::createParticleAt(int x, int y) {
	int rad = rand() % (max_particle_radius - min_particle_radius) + min_particle_radius;

	int r = rand();
	double lifespan = ((r % 200) / 200.0) * max_time;

	int dist = rand() % (max_dist / 2) + max_dist / 2;

	return std::make_shared<RocketFireParticle>(x, y, spawn_angle, rad, dist, WHITE, lifespan);
}

RocketFireSource::RocketFireSource() : RocketFireSource({ 0, 0 }, { 0, 0 }) {}

RocketFireSource::RocketFireSource(Point a, Point b) : LineParticleSource(a.x, a.y, b.x, b.y, 20, true, true) {
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