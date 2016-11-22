#ifndef _GAME_H_
#define _GAME_H_

#include "Rocket.h"

class Game {
private:
	Rocket rocket;
public:
	Game();
	void update(double seconds);
	const Rocket& getRocket();
	void turnRocket(double newDir);
};

#endif