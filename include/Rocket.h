#ifndef _ROCKET_H_
#define _ROCKET_H_

#include "Vector.h"
#include <SDL.h>

class Rocket {
private:
	const double speed = 5;	//pixels per millisec
	Point loc;
	Vector direction;

	SDL_Rect* rect;
public:
	//drawing stuff
	static constexpr char* sprite = "../sprites/PNG/playerShip3_blue.png";
	static SDL_Texture* tex;
	static int width;
	static int height;

	Rocket();
	Rocket(const Rocket& r);
	Rocket(Point loc, double direction = 0);
	~Rocket();
	Rocket& operator= (const Rocket& r);
	void step(double seconds);
	void setDir(double dir);
	Point getLoc() const;
	double getDir() const;
	void draw(SDL_Renderer* ren);
};

#endif