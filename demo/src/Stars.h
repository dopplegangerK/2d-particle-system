#ifndef _STARS_H_
#define _STARS_H_

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <memory> 
#include "Particle.h"
#include "ScatteredParticleSource.h"

class Star : public Particle {
protected:
	static constexpr int max_radius = 4;
	static constexpr int color_alpha_val = 140;
	Uint32 color;
	int radius;
public:
	Star(int x, int y);
	static std::shared_ptr<Star> createParticleAt(int x, int y);
	virtual bool is_dead() const { return false; }
	virtual void draw(SDL_Renderer* ren);
};

class StarGenerator : public ScatteredParticleSource<Star> {
public:
	StarGenerator(int x, int y, unsigned int w, unsigned int h, int numStars);
	virtual ~StarGenerator();
};

#endif