#pragma once
#include <SDL.h>
#include "Definitions.h"

class Game;

#define GUI_XMARGIN 4 // Margin from left side of window
#define GUI_YMARGIN 4 // Margin from top of window
#define GUI_SQWIDTH 64 // 520/8
#define GUI_SQSPACING 1 // Border between squares
#define GUI_SQHEIGHT GUI_SQWIDTH
#define GUI_PADDING 4 // Padding when drawing a piece in a square

#define GUI_MARKERSIZE 8

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
	SDL_Rect get_marker_rect(int x, int y) const;

public:
	GUI(Game*);
	~GUI();
	void draw_board(Board_state);
	void handle_events();
	static void wait_for_input();
};

