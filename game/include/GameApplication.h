#ifndef _GAME_APPLICATION_H_
#define _GAME_APPLICATION_H_

#include <SDL.h>
#include "Game.h"
#include "Stars.h"

class GameApplication {
private:
	bool success;
	//game-related variables
	Game game;
	StarGenerator stars;

	// window stuff
	const int screenWidth =1024;
	const int screenHeight = 640;
	SDL_Window* window = nullptr;
	SDL_Renderer* ren = nullptr;

	//initialization methods
	void initSDL();
	void createWindow();
	void createRenderer();

	//draw methods
	void render();
	void drawBackground();
	void drawRocket();
	void drawAll();
public:
	GameApplication();
	void run();
	int result();
};

#endif