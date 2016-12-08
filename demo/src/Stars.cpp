#include "Stars.h"

#include <cstdlib>
#include <SDL2_gfxPrimitives.h>

#define SOLID_WHITE 0xffffffff
#define TRANSPARENT_WHITE 0x64ffffff
#define LIGHT_WHITE 0x32ffffff
#define LIGHTER_WHITE 0x16ffffff

Star::Star(int x, int y) : Particle(x, y), twinkle_time(0) {
	// Generate a random time
	max_twinkle_time = ((double)rand() / RAND_MAX) * 0.5 + 0.75;

	// Generate a random size
	radius = rand() % max_radius;

	// Generate a random, saturated color
	uint8_t r = (uint8_t)rand();
	uint8_t b = (uint8_t)rand();
	uint8_t g = (uint8_t)rand();
		#define maxColor 255
		#define minColor 50

	if (r > b && r > g) {
		r = maxColor;
		if (g < b)
			g = minColor;
		else
			b = minColor;
	}
	else if (g > b) {
		g = maxColor;
		if (r < b)
			r = minColor;
		else
			b = minColor;
	}
	else {
		b = maxColor;
		if (g < r)
			g = minColor;
		else
			r = minColor;
	}

	uint8_t theColor[4] = { r, g, b, color_alpha_val };
	color = *(Uint32*)&theColor;
}

std::shared_ptr<Star> Star::createParticleAt(int x, int y) {
	return std::make_shared<Star>(x, y);
}

void Star::step(double seconds) {
	if (twinkle_time < 0)
		twinkle_brighter = true;
	else if(twinkle_time > max_twinkle_time)
		twinkle_brighter = false;
	
	if (twinkle_brighter)
		twinkle_time += seconds;
	else
		twinkle_time -= seconds;
}

void Star::draw(SDL_Renderer* ren) {
	int myX = (int)x;
	int myY = (int)y;

	double twinkle_amt = twinkle_time / max_twinkle_time - 0.5;
	int draw_radius = radius + twinkle_amt * 2.5;

	circleColor(ren, myX, myY, draw_radius + 1, color);
	circleColor(ren, myX, myY, draw_radius, color);
	//draw center
	filledCircleRGBA(ren, myX, myY, draw_radius, 255, 255, 255, 100 + twinkle_amt * 100);
	filledCircleRGBA(ren, myX, myY, draw_radius - 1, 255, 255, 255, 205 + twinkle_amt * 100);
}

StarGenerator::StarGenerator(int x, int y, unsigned int w, unsigned int h, int numStars) :
	ScatteredParticleSource(x, y, w, h, numStars, true, true) {}

StarGenerator::~StarGenerator() {}