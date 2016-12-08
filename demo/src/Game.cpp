#include "Game.h"

#include "Bullet.h"
#include <SDL_mixer.h>
#include <iostream>

Game::Game() : enemySpawn(1024, 640), world(new b2World(b2Vec2(0, 0))), state{ START } {
	Enemy::setPlayer(&rocket);
	Enemy::setPhysicsWorld(world);
	Rocket::setPhysicsWorld(world);
	Bullet::setPhysicsWorld(world);
}

void Game::startGame() {
	game_lock.lock();

	rocket = Rocket({ 512, 320 }, 0);
	score = 0;
	life = 3;
	enemySpawn.clear();
	state = PLAY;

	game_lock.unlock();
}

/*******************
 * Physics Updates *
 *******************/

void Game::stepPhysics(double seconds) {
	//look at collisions
	for (b2Contact* c = world->GetContactList(); c; c = c->GetNext()) {
		b2Fixture* A = c->GetFixtureA();
		b2Fixture* B = c->GetFixtureB();
		//std::cout << "collision\n";
		PhysicsData* aObj = (PhysicsData*)A->GetUserData();
		PhysicsData* bObj = (PhysicsData*)B->GetUserData();

		//Collision between rocket and enemy
		if (aObj->type == ROCKET && bObj->type == ENEMY)
			enemyHitPlayer((Enemy*)bObj->object);
		else if (bObj->type == ROCKET && aObj->type == ENEMY)
			enemyHitPlayer((Enemy*)aObj->object);

		//Collision between bullet and enemy
		else if (aObj->type == BULLET && bObj->type == ENEMY)
			bulletHitEnemy((Enemy*)bObj->object, (Bullet*)aObj->object);
		else if (bObj->type == BULLET && aObj->type == ENEMY)
			bulletHitEnemy((Enemy*)aObj->object, (Bullet*)bObj->object);

		//Collision between bullet and player
		else if (aObj->type == BULLET && bObj->type == ROCKET)
			bulletHitPlayer((Bullet*)aObj->object);
		else if (bObj->type == BULLET && aObj->type == ROCKET)
			bulletHitPlayer((Bullet*)bObj->object);
	}
	world->Step((float32)seconds, 8, 3);
}

void Game::enemyHitPlayer(Enemy* e) {
	if (rocket.canHit()) {
		life--;
		rocket.hit();
	}
	e->hit(1);
}

void Game::bulletHitPlayer(Bullet* b) {
	b->hit();
	if (rocket.canHit()) {
		life--;
		rocket.hit();
	}
}

void Game::bulletHitEnemy(Enemy* e, Bullet* b) {
	b->hit();
	e->hit(1);
	score += e->pointValue();
	score_change = true;
}

/*******************
 * General updates *
 *******************/

void Game::endGame(double seconds) {
	rocket.explode();
	rocket.step(seconds);
	enemySpawn.step_explosions(seconds);
	//std::cout << "End of game\n";
}

void Game::update(double seconds) {
	game_lock.lock();

	if (state == START) {
		game_lock.unlock();
		return;
	}

	if (life == 0) {
		state = END;
		enemySpawn.step(seconds);
		endGame(seconds);
		game_lock.unlock();
		return;
	}

	while(bullets > 0) {
		rocket.shoot();
		bullets--;
	}

	rocket.step(seconds);
	enemySpawn.step(seconds);

	physics_is_running = true;
	stepPhysics(seconds);
	physics_is_running = false;

	game_lock.unlock();
}

/**************************
 * Getters, setters, etc. *
 **************************/

Rocket& Game::getRocket() { return rocket; }

void Game::turnRocket(double newDir) {
	if(life > 0)
		rocket.setDir(newDir);
}

EnemySpawn& Game::getEnemySpawn() { return enemySpawn; }

void Game::setState(GameState new_state) {
	while(state != new_state)
		state = new_state;
}

bool Game::scoreChanged() { return score_change; }

int Game::getScore() {
	score_change = false;
	return score;
}

int Game::getLives() { return life; }

int Game::maxLives() { return max_lives; }

/***********
 * Cleanup *
 ***********/

template <class T>
void Game::cleanup_class() {
	if (T::tex != nullptr)
		SDL_DestroyTexture(T::tex);
}

void Game::cleanup() {
	cleanup_class<GreenEnemy>();
	cleanup_class<RedEnemy>();
	cleanup_class<Rocket>();
	cleanup_class<PlayerBullet>();
	cleanup_class<EnemyBullet>();
}