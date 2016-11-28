#include "GameApplication.h"

#include <iostream>
#include <string>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>

#define FAIL { success = false; return; }

#define TICK 1000/60

// Color definitions
#define BLACK 0xff000000
#define RED 0xff0000ff
#define GREEN 0xff00ff00

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

SDL_Texture* loadTexture(const char* file, SDL_Renderer *ren) {
	SDL_Texture *texture = IMG_LoadTexture(ren, file);
	if (texture == nullptr) {
		logSDLError("LoadTexture");
	}
	return texture;
}

template <class T>
void GameApplication::loadClassSprite() {
	T::tex = loadTexture(T::sprite, ren);
	int w, h;
	SDL_QueryTexture(T::tex, NULL, NULL, &w, &h);
	T::width = w/2;
	T::height = h/2;
}

GameApplication::GameApplication() : success{ true }, stars(0, 0, screenWidth, screenHeight, 100)  {
	stars.initialize_particles();
	initSDL();
	createWindow();
	createRenderer();
	loadClassSprite<Rocket>();
	loadClassSprite<Enemy>();
	loadClassSprite<Bullet>();
	Bullet::width *= 2;
	Bullet::height *= 2;
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
	stars.draw_particles(ren);
}

void GameApplication::drawRocket() {
	game.getRocket().draw(ren);
	Point loc = game.getRocket().getLoc();
	//boxColor(ren, loc.x - 10, loc.y - 10, loc.x + 10, loc.y + 10, GREEN);
}

void GameApplication::drawEnemies() {
	game.getEnemySpawn().draw_particles(ren);
}

void GameApplication::drawAll() {
	SDL_RenderClear(ren);

	drawBackground();
	drawRocket();
	drawEnemies();

	render();
}

/*****************
* Update methods *
******************/

Uint32 tick(Uint32 interval, void* args) {
	GameApplication* g = (GameApplication*)args;

	g->game.update((double)interval/1000);

	g->drawAll();

	return interval;
}

void GameApplication::run() {
	if (!success)
		return;

	game.startGame();

	//start the update timer
	SDL_AddTimer(TICK, (SDL_TimerCallback)tick, this);


	//render/input loop
	SDL_Event e;
	bool quit = false;
	bool mouseDown = false;
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
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
				if (!mouseDown) {
					game.getRocket().shoot();
					mouseDown = true;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				mouseDown = false;
				break;
			default:
				break;
			}
		}
		//drawAll();
	}
}

int GameApplication::result() {
	if (success)
		return 0;
	else
		return -1;
}