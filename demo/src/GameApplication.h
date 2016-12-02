#ifndef _GAME_APPLICATION_H_
#define _GAME_APPLICATION_H_

#include <SDL.h>
#include <SDL_ttf.h>
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

	//text
	static constexpr char* font_path = "../../demo/fonts/fixedsys.ttf";
	TTF_Font* big_font;
	TTF_Font* med_font;
	TTF_Font* small_font;

	SDL_Texture* pause1_text = nullptr;
	SDL_Rect pause1_rect;
	SDL_Rect pause1_dest;

	SDL_Texture* pause2_text = nullptr;
	SDL_Rect pause2_rect;
	SDL_Rect pause2_dest;

	SDL_Texture* score_text = nullptr;
	SDL_Rect score_rect;
	SDL_Rect score_dest;

	SDL_Texture* end_text = nullptr;
	SDL_Rect end_rect;
	SDL_Rect end_dest;

	//any textures
	static constexpr char* life_tex_path = "../../demo/sprites/playerLife3_blue.png";
	SDL_Texture* life_tex;
	SDL_Rect life_rect;

	//game-related variables
	Game game;
	StarGenerator stars;
	bool paused = false;
	//set to true after updating game state, set to false after drawing
	bool updated = true;

	SDL_TimerID timer = 0;

	//initialization methods
	void initSDL();
	void createWindow();
	void createRenderer();
	SDL_Texture* loadSprite(const char* path, SDL_Rect* rect);
	template <class T> void loadClassSprite(double scale);
	void loadFont();
	SDL_Texture* loadText(const char* text, TTF_Font* font, SDL_Color color, SDL_Rect* src);
	void loadAllText();

	//draw methods
	void render();
	void drawBackground();
	void drawRocket();
	void drawEnemies();
	void drawScore();
	void drawLives();
	void drawPauseScreen();
	void drawEndScreen();
	void drawAll();

	void cleanup();
public:
	GameApplication();
	void run();
	int result();
};

#endif