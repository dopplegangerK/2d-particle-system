#ifndef _FIRE_H_
#define _FIRE_H_

#include <Particles.h>

// Superclass for fire particle effects (including explosions).
// Not for direct use in a particle system. (no factory constructor)
class FireParticle : public TrajectoryParticle {
protected:
	int radius;
	double lifespan;
	double time_lived;
	uint8_t color[4];
public:
	FireParticle(int x, int y, double angle, int radius, int dist, uint32_t color, double lifespan);
	virtual ~FireParticle() {}
	virtual bool is_dead() const;
	virtual void step(double seconds);
	virtual void draw(SDL_Renderer* ren);
};

//specific class for fire coming off the player's ship
class RocketFireParticle : public FireParticle {
private:
	static constexpr int max_dist = 5;
	static constexpr double max_time = 0.1;
	static constexpr int max_particle_radius = 4;
	static constexpr int min_particle_radius = 1;
	static double spawn_angle;
public:
	RocketFireParticle(int x, int y, double angle, int radius, int dist, uint32_t color, double lifespan);
	virtual ~RocketFireParticle() {}

	static void set_angle(double angle);
	static std::shared_ptr<RocketFireParticle> createParticleAt(int x, int y);
};

class RocketFireSource : public LineParticleSource<RocketFireParticle> {
public:
	RocketFireSource();
	RocketFireSource(Point a, Point b);
	virtual void moveTo(Point newA, Point newB);
	virtual void generate_new_particles(int num);
};

#endif