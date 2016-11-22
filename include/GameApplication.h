#ifndef _GAME_APPLICATION_H_
#define _GAME_APPLICATION_H_

#include <SDL.h>
#include "Game.h"

class GameApplication {
private:
	// window stuff
	const int screenWidth =1024;
	const int screenHeight = 640;
	SDL_Window* window = nullptr;
	SDL_Renderer* ren = nullptr;

	bool success;
	Game game;

	//initialization methods
	void initSDL();
	void createWindow();
	void createRenderer();
public:
	GameApplication();
	void run();
	int result();
};

#endif