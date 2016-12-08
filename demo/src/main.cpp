#include <SDL.h>
#include <iostream>
#include "GameApplication.h"

int main(int argc, char* argv[]) {
	srand((unsigned int)time(0));
	GameApplication game;
	game.run();
	return game.result();
}
