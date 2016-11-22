#ifndef _GAME_APPLICATION_H_
#define _GAME_APPLICATION_H_

#include <SDL.h>
#include "Game.h"

class GameApplication {
private:
	bool success;

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

	//game-related variables
	Game game;
	Point mouseLoc = { 0, 0 };
public:
	GameApplication();
	void run();
	int result();
};

#endif