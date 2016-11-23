#include "GameApplication.h"

#include <iostream>
#include <string>
#include <SDL2_gfxPrimitives.h>

#define FAIL { success = false; return; }

#define TICK 1000/60

// Color definitions
#define BLACK 0xff000000
#define RED 0xff0000ff

void logSDLError(const std::string &msg) {
	std::cout << msg << " error: " << SDL_GetError() << std::endl;
}

/******************
 * Initialization *
 ******************/

void GameApplication::initSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logSDLError("SDL_Init Error: ");
		FAIL;
	}
	//init mixer, etc
}

void GameApplication::createWindow() {
	window = SDL_CreateWindow(
		"Final Project",                  // window title (think of a better name later)
		8,           // initial x position
		36,           // initial y position
		screenWidth,                               // width, in pixels
		screenHeight,                               // height, in pixels
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI             // flags
	);

	// Check that the window was successfully created
	if (window == nullptr) {
		logSDLError("SDL_CreateWindowError: ");
		SDL_Quit();
		FAIL;
	}
}

void GameApplication::createRenderer() {
	ren = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
	if (ren == nullptr) {
		SDL_DestroyWindow(window);
		logSDLError("SDL_CreateRenderer Error: ");
		SDL_Quit();
		FAIL;
	}
}

GameApplication::GameApplication() : success{ true }  {
	initSDL();
	createWindow();
	createRenderer();
}

/***************
* Draw methods *
****************/

void GameApplication::render() {
	if (!success)
		FAIL;
	SDL_RenderPresent(ren);
}

void GameApplication::drawBackground() {
	boxColor(ren, 0, 0, screenWidth, screenHeight, BLACK);
}

void GameApplication::drawRocket() {
	Point loc = game.getRocket().getLoc();
	boxColor(ren, loc.x - 10, loc.y - 10, loc.x + 10, loc.y + 10, RED);
}

void GameApplication::drawAll() {
	SDL_RenderClear(ren);

	drawBackground();
	drawRocket();

	render();
}

/*****************
* Update methods *
******************/

Uint32 tick(Uint32 interval, void* args) {
	Game* game = (Game*)args;

	game->update((double)interval/1000);

	return interval;
}

void GameApplication::run() {
	if (!success)
		return;

	//start the update timer
	SDL_AddTimer(TICK, (SDL_TimerCallback)tick, &game);


	//render/input loop
	SDL_Event e;
	bool quit = false;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_MOUSEMOTION: {
				int mouseX, mouseY; 
				SDL_GetMouseState(&mouseX, &mouseY);
				game.turnRocket(Vector(game.getRocket().getLoc(), { mouseX, mouseY }).getAngle());
			}
			default:
				break;
			}
		}
		drawAll();
	}
}

int GameApplication::result() {
	if (success)
		return 0;
	else
		return -1;
}