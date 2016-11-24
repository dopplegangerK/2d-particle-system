#include <SDL.h>
#include <iostream>
#include "GameApplication.h"

int main(int argc, char* argv[]) {
	GameApplication game;
	game.run();
	return game.result();
}
