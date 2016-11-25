#include "Game.h"

Game::Game() : rocket{ {512, 320}, 0 }, enemySpawn(1024, 640) {
	Enemy::setPlayer(&rocket);
}

void Game::update(double seconds) {
	rocket.step(seconds);
	enemySpawn.step(seconds);
}

const Rocket& Game::getRocket() { return rocket; }

void Game::turnRocket(double newDir) { rocket.setDir(newDir); }

EnemySpawn& Game::getEnemySpawn() { return enemySpawn; }