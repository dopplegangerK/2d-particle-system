#ifndef _GAME_H_
#define _GAME_H_

#include "Rocket.h"
#include "Enemy.h"
#include <Box2D\Box2D.h>

enum PhysicsObjType {
	ENEMY = 1,
	ROCKET = 2
};

class Game {
private:
	Rocket rocket;
	EnemySpawn enemySpawn;
	b2World* world;

	int life = 3;

	void stepPhysics(double seconds);
public:
	Game();

	void startGame();

	void update(double seconds);

	void endGame();

	Rocket& getRocket();
	void turnRocket(double newDir);
	EnemySpawn& getEnemySpawn();
};

#endif