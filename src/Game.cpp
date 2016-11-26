#include "Game.h"

Game::Game() : enemySpawn(1024, 640) {
	Enemy::setPlayer(&rocket);
}

void Game::makeNewRocket() {
	rocket = Rocket({ 512, 320 }, 0);
}

void Game::update(double seconds) {
	rocket.step(seconds);
	enemySpawn.step(seconds);
}

Rocket& Game::getRocket() { return rocket; }

void Game::turnRocket(double newDir) { rocket.setDir(newDir); }

EnemySpawn& Game::getEnemySpawn() { return enemySpawn; }