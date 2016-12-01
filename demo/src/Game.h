#ifndef _GAME_H_
#define _GAME_H_

#include "Rocket.h"
#include "Enemy.h"
#include <Box2D\Box2D.h>
#include <mutex>
#include <SDL_mixer.h>

enum PhysicsObjType {
	ENEMY = 1,
	ROCKET = 2,
	BULLET = 3
};

class Game {
private:
	Rocket rocket;
	EnemySpawn enemySpawn;
	b2World* world;

	int life = 300;

	bool physics_is_running = false;
	int bullets = 0;

	void stepPhysics(double seconds);

	void enemyHitPlayer(Enemy* e);
	void bulletHitEnemy(Enemy* e, Bullet* b);
public:
	std::mutex game_lock;

	Game();

	void startGame();

	void update(double seconds);

	void endGame();

	Rocket& getRocket();
	void turnRocket(double newDir);

	EnemySpawn& getEnemySpawn();

	//make all members delete their textures, etc
	template <class T> void cleanup_class();
	void cleanup();
};

#endif