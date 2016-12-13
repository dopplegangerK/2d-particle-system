#ifndef _FIRE_H_
#define _FIRE_H_

#include <Particles.h>

using namespace Particles;

// Superclass for fire particle effects (including explosions).
// Not for direct use in a particle system (has no factory constructor)
class FireParticle : public TrajectoryParticle {
protected:
	int radius;
	double lifespan;
	double time_lived;
	uint8_t color[4];
	int alpha;
	
	SDL_Rect rect;

	virtual void fade();
public:
	//drawing stuff (public so we can set it from outside)
	static constexpr char* sprite = "../../demo/sprites/fireParticle.png";
	static SDL_Texture* tex;
	static int width;
	static int height;

	FireParticle(float x, float y, double angle, int radius, int dist, uint8_t color[4], double lifespan);
	virtual ~FireParticle() {}
	virtual bool isDead() const;
	virtual void step(double seconds);
	virtual void draw(SDL_Renderer* ren);
};

// Specific class for fire coming off the player's ship
class RocketFireParticle : public FireParticle {
private:
	double r;
	double g;
	double b;
	uint8_t start_color[4];

	static constexpr int max_dist = 2;
	static constexpr double max_time = 0.15;
	static constexpr int max_particle_radius = 7;
	static constexpr int min_particle_radius = 3;
	static double spawn_angle;
public:
	RocketFireParticle(float x, float y, double angle, int radius, int dist, uint8_t color[4], double lifespan);
	virtual ~RocketFireParticle() {}
	virtual void fade();

	static void setAngle(double angle);
	static std::shared_ptr<RocketFireParticle> createParticleAt(float x, float y);
};

class RocketFireSource : public LineParticleSource<RocketFireParticle> {
public:
	RocketFireSource();
	RocketFireSource(Point a, Point b);
	virtual void moveTo(Point newA, Point newB);
	virtual void generateNewParticles(int num);
};

#endif
