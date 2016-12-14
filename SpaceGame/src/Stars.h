#ifndef _STARS_H_
#define _STARS_H_

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <memory> 
#include "Particle.h"
#include "ScatteredParticleSource.h"

using namespace Particles;

class Star : public Particle {
protected:
	double twinkle_time;
	double max_twinkle_time;
	bool twinkle_brighter;

	static constexpr int max_radius = 6;
	uint8_t color[3];
	int radius;
public:
	//drawing stuff (public so we can set it from outside)
	static constexpr char* sprite = "../../SpaceGame/sprites/fireParticle.png";
	static SDL_Texture* tex;
	static int width;
	static int height;

	Star(float x, float y);
	static std::shared_ptr<Star> createParticleAt(float x, float y);
	virtual bool isDead() const { return false; }
	virtual void step(double seconds);
	virtual void draw(SDL_Renderer* ren);
};

class StarGenerator : public ScatteredParticleSource<Star> {
public:
	StarGenerator(int x, int y, unsigned int w, unsigned int h, int numStars);
	virtual ~StarGenerator();
};

#endif
