#ifndef _GAME_H_
#define _GAME_H_

#include "Rocket.h"
#include "Enemy.h"
#include "Meteor.h"
#include <Box2D/Box2D.h>
#include <mutex>
#include <SDL_mixer.h>

enum PhysicsObjType {
	ENEMY = 1,
	ROCKET = 2,
	BULLET = 3,
        METEOR = 4
};

enum GameState {
	START,
	PLAY,
	END
};

class Game {
private:
	GameState state = PLAY;

	Rocket rocket;
	EnemySpawn enemySpawn;
        MeteorSpawn meteorSpawn;
	b2World* world;

	int score = 0;
	bool score_change = false;

	bool physics_is_running = false;
	int bullets = 0;

	void stepPhysics(double seconds);

	void enemyHitPlayer(Enemy* e);
	void bulletHitPlayer(Bullet* b);
	void bulletHitEnemy(Enemy* e, Bullet* b);
        void meteorHit(PhysicsData* obj);
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
        MeteorSpawn& getMeteorSpawn();

	void setState(GameState new_state);

	bool scoreChanged();
	int getScore();

	int maxLives();
	int getLives();

	//make all members delete their textures, etc
	template <class T> void cleanup_class();
	void cleanup();
};

#endif
