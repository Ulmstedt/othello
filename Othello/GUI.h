#pragma once
#include <SDL.h>

class GUI
{
	// The window we'll be rendering to
	SDL_Window *window = NULL;

	// The surface contained by the window
	SDL_Surface *screenSurface = NULL;

	// The board image
	SDL_Surface *boardSurface = NULL;

	// Event handler
	SDL_Event e;

	bool init();
	bool loadMedia();
	void close();

public:
	GUI();
	~GUI();
	void draw_board();
	void handle_events();
};

