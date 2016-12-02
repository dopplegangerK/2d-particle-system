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

enum GameState {
	PLAY,
	END
};

class Game {
private:
	GameState state = PLAY;

	Rocket rocket;
	EnemySpawn enemySpawn;
	b2World* world;

	static constexpr int max_lives = 3;
	int life = max_lives;
	int score = 0;
	bool score_change = false;

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

	void endGame(double seconds);

	GameState getState() { return state; }

	Rocket& getRocket();
	void turnRocket(double newDir);

	EnemySpawn& getEnemySpawn();

	bool scoreChanged();
	int getScore();

	int maxLives();
	int getLives();

	//make all members delete their textures, etc
	template <class T> void cleanup_class();
	void cleanup();
};

#endif