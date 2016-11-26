#ifndef _GAME_H_
#define _GAME_H_

#include "Rocket.h"
#include "Enemy.h"
#include <Box2D\Box2D.h>

class Game {
private:
	Rocket rocket;
	EnemySpawn enemySpawn;
	b2World* world;
public:
	Game();
	void makeNewRocket();
	void update(double seconds);
	Rocket& getRocket();
	void turnRocket(double newDir);
	EnemySpawn& getEnemySpawn();
};

#endif