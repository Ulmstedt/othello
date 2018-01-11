#pragma once
#include <SDL.h>
#include "Definitions.h"
//#include "Game.h"

class Game;

class GUI
{
	// The window we'll be rendering to
	SDL_Window *window = NULL;

	//The window renderer
	SDL_Renderer* renderer = NULL;

	// The surface contained by the window
	SDL_Surface *screenSurface = NULL;

	// The board texture
	SDL_Texture *boardTexture = NULL;

	// Event handler
	SDL_Event e;

	// The game this GUI represents
	Game *game = NULL;

	bool init();
	bool loadMedia();
	void close();
	SDL_Rect get_rect(int x, int y) const;

public:
	GUI(Game*);
	~GUI();
	void draw_board(Board_state);
	void handle_events();
};

