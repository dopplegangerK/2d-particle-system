#include "GameApplication.h"

#include <iostream>
#include <string>
#include <SDL2_gfxPrimitives.h>
#include <SDL_image.h>

#define FAIL  {success = false; return;}

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
	/*
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		logSDLError("Mixer_Init");
		SDL_Quit();
		FAIL;
	}
	*/
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

SDL_Texture* GameApplication::loadSprite(const char* path, SDL_Rect* rect) {
	rect->x = 0;
	rect->y = 0;
	SDL_Texture* tex = loadTexture(path, ren);
	SDL_QueryTexture(tex, NULL, NULL, &rect->w, &rect->h);
	return tex;
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
	small_font = openFont(font_path, 20);
}

Text GameApplication::loadText(const char* text, TTF_Font* font, SDL_Color color, Point p, bool centered) {
	Text t;
	t.tex = renderText(text, font, color, ren);
	t.src.x = 0;
	t.src.y = 0;
	SDL_QueryTexture(t.tex, NULL, NULL, &t.src.w, &t.src.h);
	t.dest.w = t.src.w;
	t.dest.h = t.src.h;
	if (centered) {
		t.dest.x = p.x - t.dest.w / 2;
		t.dest.y = p.y - t.dest.h / 2;
	} else {
		t.dest.x = p.x;
		t.dest.y = p.y;
	}
	return t;
}

void GameApplication::loadAllText() {
	start_text = loadText("Press any key to start", med_font, WHITE_SDL_COLOR, { screenWidth / 2, screenHeight / 2 });
	pause1_text = loadText("PAUSED", big_font, WHITE_SDL_COLOR, { screenWidth / 2, screenHeight / 2 });
	pause2_text = loadText("ESC to resume", med_font, WHITE_SDL_COLOR, { screenWidth / 2, screenHeight / 2 + 55 });
	end1_text = loadText("GAME OVER", big_font, WHITE_SDL_COLOR, { screenWidth / 2, screenHeight / 2 - 20 });
	end2_text = loadText("R to Restart", med_font, WHITE_SDL_COLOR, { screenWidth / 2, end1_text.dest.y + end1_text.dest.h/2 + 50 });
	end3_text = loadText("Q to Quit", med_font, WHITE_SDL_COLOR, { screenWidth / 2, end2_text.dest.y + end2_text.dest.h / 2 + 40 });
}

GameApplication::GameApplication() : success{ true }, stars(0, 0, screenWidth, screenHeight, 100)  {
	stars.initialize_particles();
	initSDL();
	createWindow();
	createRenderer();

	loadClassSprite<Rocket>(0.5);
	loadClassSprite<Enemy>(0.5);
	loadClassSprite<Bullet>(1);
	life_tex = loadSprite(life_tex_path, &life_rect);

	loadFont();
	loadAllText();
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

void GameApplication::drawText(Text& t) {
	SDL_RenderCopy(ren, t.tex, &t.src, &t.dest);
}

void GameApplication::drawScore() {
	if (game.scoreChanged() || score_text.tex == nullptr) {
		std::string score_str = "Score: ";
		score_str += std::to_string(game.getScore());
		score_text = loadText(score_str.c_str(), med_font, WHITE_SDL_COLOR, { screenWidth - 185, screenHeight - 45, }, false);
	}
	drawText(score_text);
}

void GameApplication::drawLives() {
	SDL_Rect dest;
	dest.w = life_rect.w;
	dest.h = life_rect.h;
	dest.y = screenHeight - dest.h - 20;
	int k = 0;
	while (k < game.getLives()) {
		dest.x = k * dest.w + (k + 1) * 15;
		SDL_RenderCopy(ren, life_tex, &life_rect, &dest);
		k++;
	}
}

void GameApplication::drawStartScreen() {
	drawText(start_text);
}

void GameApplication::drawPauseScreen() {
	boxColor(ren, 0, 0, screenWidth, screenHeight, PAUSE_COLOR);
	drawText(pause1_text);
	drawText(pause2_text);
}

void GameApplication::drawEndScreen() {
	drawText(end1_text);
	drawText(end2_text);
	drawText(end3_text);
}

void GameApplication::drawAll() {
	SDL_RenderClear(ren);

	drawBackground();

	if (game.getState() != START) {
		game.game_lock.lock();

		drawRocket();
		drawEnemies();
		drawScore();
		drawLives();

		game.game_lock.unlock();

		if (paused && game.getState() == PLAY)
			drawPauseScreen();

		if (game.getState() == END)
			drawEndScreen();
	} else {
		drawStartScreen();
	}

	render();
}

/*****************
* Update methods *
******************/

void GameApplication::startGame() {
	paused = false;
	game.startGame();
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	game.turnRocket(Vector(game.getRocket().getLoc(), { mouseX, mouseY }).getAngle());
}

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
				if (game.getState() == START) {
					startGame();
				} else if (!mouseDown && !paused) {
					game.getRocket().shoot();
					mouseDown = true;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				mouseDown = false;
				break;
			case SDL_KEYDOWN:
				switch (game.getState()) {
					case START:
						startGame();
						break;
					case PLAY:
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
					case END:
						if (e.key.keysym.sym == SDLK_q)
							quit = true;
						else if (e.key.keysym.sym == SDLK_r)
							game.setState(START);
						break;
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