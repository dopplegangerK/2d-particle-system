#include "GameApplication.h"

#include <iostream>
#include <string>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>

#define FAIL { success = false; return; }

#define TICK 1000/60

// Color definitions
#define BLACK 0xff000000
#define PAUSE_COLOR 0x88000000

#define WHITE_SDL_COLOR SDL_Color{255, 255, 255, 255}

void logSDLError(const std::string &msg) {
	std::cout << msg << " error: " << SDL_GetError() << std::endl;
}


SDL_Texture* loadTexture(const char* file, SDL_Renderer *ren) {
	SDL_Texture *texture = IMG_LoadTexture(ren, file);
	if (texture == nullptr) {
		logSDLError("LoadTexture");
	}
	return texture;
}

TTF_Font* openFont(const char* fontFile, int fontSize) {
	TTF_Font* font = TTF_OpenFont(fontFile, fontSize);
	if (font == nullptr) {
		logSDLError("TTF_OpenFont");
		return nullptr;
	}
	return font;
}

SDL_Texture* renderText(const std::string &message, TTF_Font* font,
	SDL_Color color, SDL_Renderer *renderer) {
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == nullptr) {
		TTF_CloseFont(font);
		logSDLError("TTF_RenderText");
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr) {
		logSDLError("CreateTexture");
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	return texture;
}

/******************
 * Initialization *
 ******************/

void GameApplication::initSDL() {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		logSDLError("SDL_Init Error: ");
		FAIL;
	}

	if (TTF_Init() != 0) {
		logSDLError("TTF_Init");
		SDL_Quit();
		FAIL;
	}
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

template <class T>
void GameApplication::loadClassSprite(double scale) {
	T::tex = loadTexture(T::sprite, ren);
	int w, h;
	SDL_QueryTexture(T::tex, NULL, NULL, &w, &h);
	T::width = (int)(w * scale);
	T::height = (int)(h * scale);
}

void GameApplication::loadFont() {
	big_font = openFont(font_path, 72);
	med_font = openFont(font_path, 32);
}

void GameApplication::loadText() {
	pause1_text = renderText("PAUSED", big_font, WHITE_SDL_COLOR, ren);
	pause1_rect.x = 0;
	pause1_rect.y = 0;
	SDL_QueryTexture(pause1_text, NULL, NULL, &pause1_rect.w, &pause1_rect.h);
	pause1_dest = pause1_rect;
	pause1_dest.x = screenWidth / 2 - pause1_rect.w / 2;
	pause1_dest.y = screenHeight / 2 - pause1_rect.h / 2;

	pause2_text = renderText("ESC to resume", med_font, WHITE_SDL_COLOR, ren);
	pause2_rect.x = 0;
	pause2_rect.y = 0;
	SDL_QueryTexture(pause2_text, NULL, NULL, &pause2_rect.w, &pause2_rect.h);
	pause2_dest = pause2_rect;
	pause2_dest.x = screenWidth / 2 - pause2_rect.w / 2;
	pause2_dest.y = screenHeight / 2 - pause2_rect.h / 2 + 50;
}

GameApplication::GameApplication() : success{ true }, stars(0, 0, screenWidth, screenHeight, 100)  {
	stars.initialize_particles();
	initSDL();
	createWindow();
	createRenderer();
	loadClassSprite<Rocket>(0.5);
	loadClassSprite<Enemy>(0.5);
	loadClassSprite<Bullet>(1);
	loadFont();
	loadText();
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
}

void GameApplication::drawEnemies() {
	game.getEnemySpawn().draw_particles(ren);
}

void GameApplication::drawPauseScreen() {
	boxColor(ren, 0, 0, screenWidth, screenHeight, PAUSE_COLOR);
	SDL_RenderCopy(ren, pause1_text, &pause1_rect, &pause1_dest);
	SDL_RenderCopy(ren, pause2_text, &pause2_rect, &pause2_dest);
}

void GameApplication::drawAll() {
	game.game_lock.lock();

	SDL_RenderClear(ren);

	drawBackground();
	drawRocket();
	drawEnemies();

	game.game_lock.unlock();

	if (paused)
		drawPauseScreen();

	render();
}

/*****************
* Update methods *
******************/

Uint32 tick(Uint32 interval, void* args) {
	GameApplication* g = (GameApplication*)args;

	if (g->paused) {
		g->updated = true;
		return interval;
	}

	g->game.update((double)interval/1000);
	g->updated = true;

	return interval;
}

void GameApplication::run() {
	if (!success)
		return;

	game.startGame();

	//start the update timer
	timer = SDL_AddTimer(TICK, (SDL_TimerCallback)tick, this);


	//render/input loop
	SDL_Event e;
	bool quit = false;
	bool mouseDown = false;
	bool escDown = false;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_MOUSEMOTION: {
				if (!paused) {
					int mouseX, mouseY;
					SDL_GetMouseState(&mouseX, &mouseY);
					game.turnRocket(Vector(game.getRocket().getLoc(), { mouseX, mouseY }).getAngle());
				}
				break;
			}
			case SDL_MOUSEBUTTONDOWN:
				if (!mouseDown && !paused) {
					game.getRocket().shoot();
					mouseDown = true;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				mouseDown = false;
				break;
			case SDL_KEYDOWN:
				if (e.key.keysym.sym == SDLK_ESCAPE && !escDown) {
					paused = !paused;
					escDown = true;
					if (!paused) {
						int mouseX, mouseY;
						SDL_GetMouseState(&mouseX, &mouseY);
						game.turnRocket(Vector(game.getRocket().getLoc(), { mouseX, mouseY }).getAngle());
					}
				}
				break;
			case SDL_KEYUP:
				if (e.key.keysym.sym == SDLK_ESCAPE)
					escDown = false;
			default:
				break;
			}
		}
		if (updated) {
			drawAll();
			updated = false;
		}
	}
	//clean up SDL you silly person
	cleanup();
}

void GameApplication::cleanup() {
	if(timer != 0)
		SDL_RemoveTimer(timer);
	if (window != nullptr)
		SDL_DestroyWindow(window);
	if (ren != nullptr)
		SDL_DestroyRenderer(ren);
	game.cleanup();
	SDL_Quit();
}

int GameApplication::result() {
	if (success)
		return 0;
	else
		return -1;
}