#include "Stars.h"

#include <cstdlib>
#include <SDL2_gfxPrimitives.h>

#define SOLID_WHITE 0xffffffff
#define TRANSPARENT_WHITE 0x64ffffff
#define LIGHT_WHITE 0x32ffffff
#define LIGHTER_WHITE 0x16ffffff

int Star::height = 0;
int Star::width = 0;
SDL_Texture* Star::tex = nullptr;

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

	color[0] = r;
        color[1] = g;
        color[2] = b;
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

        SDL_Rect outer;
        outer.w = draw_radius * 2 + 4;
        outer.h = draw_radius * 2 + 4;
        outer.x = (int)x - outer.w/2;
        outer.y = (int)y- outer.h/2;
        SDL_SetTextureColorMod(tex, color[0], color[1], color[2]);
        SDL_SetTextureAlphaMod(tex, 255);
	SDL_RenderCopy(ren, tex, NULL, &outer);

        SDL_Rect inner;
        inner.w = draw_radius * 2;
        inner.h = draw_radius * 2;
        inner.x = (int)x - inner.w/2;
        inner.y = (int)y- inner.h/2;
        SDL_SetTextureColorMod(tex, 255, 255, 255);
        SDL_SetTextureAlphaMod(tex, 205 + twinkle_amt * 100);
	SDL_RenderCopy(ren, tex, NULL, &inner);

        /*
	circleColor(ren, myX, myY, draw_radius + 1, color);
	circleColor(ren, myX, myY, draw_radius, color);
	//draw center
	filledCircleRGBA(ren, myX, myY, draw_radius, 255, 255, 255, 100 + twinkle_amt * 100);
	filledCircleRGBA(ren, myX, myY, draw_radius - 1, 255, 255, 255, 205 + twinkle_amt * 100);
        */
}

StarGenerator::StarGenerator(int x, int y, unsigned int w, unsigned int h, int numStars) :
	ScatteredParticleSource(x, y, w, h, numStars, true, true) {}

StarGenerator::~StarGenerator() {}
