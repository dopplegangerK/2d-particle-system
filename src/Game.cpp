#include "Game.h"

Game::Game() : enemySpawn(1024, 640), world(new b2World(b2Vec2(0, 0))) {
	Enemy::setPlayer(&rocket);
	Enemy::setPhysicsWorld(world);
	Rocket::setPhysicsWorld(world);
}

void Game::makeNewRocket() {
	rocket = Rocket({ 512, 320 }, 0);
}

void Game::update(double seconds) {
	world->Step(1.0/60, 8, 3);
	rocket.step(seconds);
	enemySpawn.step(seconds);
}

Rocket& Game::getRocket() { return rocket; }

void Game::turnRocket(double newDir) { rocket.setDir(newDir); }

EnemySpawn& Game::getEnemySpawn() { return enemySpawn; }