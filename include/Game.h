#ifndef _GAME_H_
#define _GAME_H_

#include "Rocket.h"
#include "Enemy.h"

class Game {
private:
	Rocket rocket;
	EnemySpawn enemySpawn;
public:
	Game();
	void makeNewRocket();
	void update(double seconds);
	Rocket& getRocket();
	void turnRocket(double newDir);
	EnemySpawn& getEnemySpawn();
};

#endif