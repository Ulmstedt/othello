#include "stdafx.h"
#include "GUI.h"
#include <SDL.h>
#include <iostream>
#include "Game.h"

#define SCREEN_WIDTH 528
#define SCREEN_HEIGHT 528

using namespace std;

bool GUI::init()
{
	// Initialization flag
	bool success = true;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << endl;
		success = false;
	}
	else
	{
		// Create window
		window = SDL_CreateWindow("Othello by Mattias Ulmstedt", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			cout << "Window could not be created! SDL_Error: " << SDL_GetError() << endl;
			success = false;
		}
		else
		{
			// Get window surface
			screenSurface = SDL_GetWindowSurface(window);
		}
	}

	return success;
}

bool GUI::loadMedia()
{
	// Loading success flag
	bool success = true;

	// Load board image
	boardSurface = SDL_LoadBMP("img/board.bmp");
	if (boardSurface == NULL)
	{
		cout << "Unable to load board image! SDL_Error: " << SDL_GetError() << endl;
		success = false;
	}

	return success;
}

void GUI::close()
{
	// Deallocate surface
	SDL_FreeSurface(boardSurface);
	boardSurface = NULL;

	// Destroy window
	SDL_DestroyWindow(window);

	// Quite SDL subsystems
	SDL_Quit();
}

GUI::GUI()
{
	init();
	loadMedia();
}


GUI::~GUI()
{
	close();
}

void GUI::draw_board()
{
	SDL_BlitSurface(boardSurface, NULL, screenSurface, NULL);

	// Update surface
	SDL_UpdateWindowSurface(window);
}

void GUI::handle_events()
{
	while (SDL_PollEvent(&e) != 0)
	{
		// Quit event
		if (e.type == SDL_QUIT)
		{
			Game::quit_game();
		}
	}
}
