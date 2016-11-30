#ifndef _GAME_APPLICATION_H_
#define _GAME_APPLICATION_H_

#include <SDL.h>
#include "Game.h"
#include "Stars.h"

class GameApplication {
private:
	friend Uint32 tick(Uint32 interval, void* args);

	bool success;

	// window stuff
	const int screenWidth =1024;
	const int screenHeight = 640;
	SDL_Window* window = nullptr;
	SDL_Renderer* ren = nullptr;

	//game-related variables
	Game game;
	StarGenerator stars;
	//set to true after updating game state, set to false after drawing
	bool updated = true;

	//initialization methods
	void initSDL();
	void createWindow();
	void createRenderer();
	template <class T> void loadClassSprite(double scale);

	//draw methods
	void render();
	void drawBackground();
	void drawRocket();
	void drawEnemies();
	void drawAll();
public:
	GameApplication();
	void run();
	int result();
};

#endif