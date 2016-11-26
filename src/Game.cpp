#include "Game.h"

#include <iostream>

Game::Game() : enemySpawn(1024, 640), world(new b2World(b2Vec2(0, 0))) {
	Enemy::setPlayer(&rocket);
	Enemy::setPhysicsWorld(world);
	Rocket::setPhysicsWorld(world);
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
		//For now, just reduce life if the rocket was hit
		if (aObj->type == ROCKET) {
			life--;
			if (bObj->type == ENEMY) {
				Enemy* e = (Enemy*)bObj->object;
				e->hit(3);
			}
		} else if (bObj->type == ROCKET) {
			life--;
			if (aObj->type == ENEMY) {
				Enemy* e = (Enemy*)aObj->object;
				e->hit(3);
			}
		}
	}
}

void Game::update(double seconds) {
	stepPhysics(seconds);

	if (life == 0) {
		endGame();
		return;
	}

	rocket.step(seconds);
	enemySpawn.step(seconds);
}

Rocket& Game::getRocket() { return rocket; }

void Game::turnRocket(double newDir) { rocket.setDir(newDir); }

EnemySpawn& Game::getEnemySpawn() { return enemySpawn; }

void Game::endGame() {
	std::cout << "End of game\n";
}