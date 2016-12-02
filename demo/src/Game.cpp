#include "Game.h"
#include "Bullet.h"

#include <iostream>

Game::Game() : enemySpawn(1024, 640), world(new b2World(b2Vec2(0, 0))) {
	Enemy::setPlayer(&rocket);
	Enemy::setPhysicsWorld(world);
	Rocket::setPhysicsWorld(world);
	Bullet::setPhysicsWorld(world);
}

void Game::startGame() {
	rocket = Rocket({ 512, 320 }, 0);
}

void Game::stepPhysics(double seconds) {
	world->Step((float32)seconds, 8, 3);
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
		if (aObj->type == BULLET && bObj->type == ENEMY)
			bulletHitEnemy((Enemy*)bObj->object, (Bullet*)aObj->object);
		else if(bObj->type == BULLET && aObj->type == ENEMY)
			bulletHitEnemy((Enemy*)aObj->object, (Bullet*)bObj->object);
	}
}

void Game::enemyHitPlayer(Enemy* e) {
	life--;
	rocket.hit();
	e->hit(3);
}

void Game::bulletHitEnemy(Enemy* e, Bullet* b) {
	b->hit();
	e->hit(3);
	score++;
	score_change = true;
}

void Game::update(double seconds) {
	game_lock.lock();

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

Rocket& Game::getRocket() { return rocket; }

void Game::turnRocket(double newDir) {
	if(life > 0)
		rocket.setDir(newDir);
}

EnemySpawn& Game::getEnemySpawn() { return enemySpawn; }

bool Game::scoreChanged() { return score_change; }

int Game::getScore() {
	score_change = false;
	return score;
}

int Game::getLives() { return life; }

int Game::maxLives() { return max_lives; }

void Game::endGame(double seconds) {
	rocket.explode();
	rocket.step(seconds);
	enemySpawn.step_explosions(seconds);
	//std::cout << "End of game\n";
}

template <class T>
void Game::cleanup_class() {
	if (T::tex != nullptr)
		SDL_DestroyTexture(T::tex);
}

void Game::cleanup() {
	cleanup_class<Enemy>();
	cleanup_class<Rocket>();
	cleanup_class<Bullet>();
}