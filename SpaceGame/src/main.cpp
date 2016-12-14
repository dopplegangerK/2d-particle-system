#include <SDL.h>
#include "GameApplication.h"

int main(int argc, char* argv[]) {
	srand((unsigned int)time(0));
	GameApplication game;
	game.run();
	return game.result();
}
