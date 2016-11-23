#include "Game.h"

Game::Game() : rocket{ {512, 320}, 0 } {}

void Game::update(double seconds) {
	rocket.step(seconds);
}

const Rocket& Game::getRocket() { return rocket; }

void Game::turnRocket(double newDir) { rocket.setDir(newDir); }