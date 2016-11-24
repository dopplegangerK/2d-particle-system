#include "Stars.h"

#include <cstdlib>
#include <SDL2_gfxPrimitives.h>

#define SOLID_WHITE 0xffffffff
#define TRANSPARENT_WHITE 0x64ffffff
#define LIGHT_WHITE 0x32ffffff
#define LIGHTER_WHITE 0x16ffffff

Star::Star(int x, int y) : Particle(x, y) {
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

void Star::draw(SDL_Renderer* ren) {
	//filledCircleColor(ren, x, y, (radius + 1) * 4, LIGHTER_WHITE);
	//filledCircleColor(ren, x, y, (radius + 1) * 2, LIGHTER_WHITE);

	circleColor(ren, x, y, radius + 1, color);
	circleColor(ren, x, y, radius, color);
	//draw center
	filledCircleColor(ren, x, y, radius, TRANSPARENT_WHITE);
	filledCircleColor(ren, x, y, radius - 1, SOLID_WHITE);

}

StarGenerator::StarGenerator(int x, int y, unsigned int w, unsigned int h, int numStars) :
	ScatteredParticleSource(x, y, w, h, numStars, false, true) {}

StarGenerator::~StarGenerator() {}