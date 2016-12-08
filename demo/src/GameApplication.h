#ifndef _GAME_APPLICATION_H_
#define _GAME_APPLICATION_H_

#include <SDL.h>
#include <SDL_ttf.h>
#include "Game.h"
#include "Stars.h"

struct Text {
	SDL_Texture* tex = nullptr;
	SDL_Rect src;
	SDL_Rect dest;
};

class GameApplication {
private:
	friend Uint32 tick(Uint32 interval, void* args);

	static constexpr double TICK = 1000 / 60;

	bool success;

	// window stuff
	const int screenWidth =1024;
	const int screenHeight = 640;
	SDL_Window* window = nullptr;
	SDL_Renderer* ren = nullptr;

	// Stuff for text
	static constexpr char* font_path = "../../demo/fonts/fixedsys.ttf";
	TTF_Font* big_font;
	TTF_Font* med_font;
	TTF_Font* small_font;

	Text start_text;
	Text pause1_text;
	Text pause2_text;
	Text score_text;
	Text end1_text;
	Text end2_text;
	Text end3_text;

	// sound paths
	static constexpr char* explosion_sound = "../../demo/sounds/Explosion+7.wav";

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
	Text loadText(const char* text, TTF_Font* font, SDL_Color color, Point p, bool centered = true);
	void loadAllText();

	//draw methods
	void render();
	void drawBackground();
	void drawRocket();
	void drawEnemies();
	void drawText(Text& t);
	void drawScore();
	void drawLives();
	void drawStartScreen();
	void drawPauseScreen();
	void drawEndScreen();
	void drawAll();

	void startGame();

	void cleanup();
public:
	GameApplication();
	void run();
	int result();
};

#endif