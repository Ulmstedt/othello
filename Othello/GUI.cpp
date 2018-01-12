#include "stdafx.h"
#include "GUI.h"
#include <SDL.h>
#include <iostream>
#include "Game.h"
#include "BoardUtil.h"

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
			// Create window renderer
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (renderer == NULL)
			{
				cout << "Renderer could not be created! SDL_Error: " << SDL_GetError() << endl;
				success = false;
			}
		}
	}

	return success;
}

bool GUI::loadMedia()
{
	// Loading success flag
	bool success = true;

	// Temp surface to hold the surface
	SDL_Surface *tempSurface = NULL;

	// Load board image
	tempSurface = SDL_LoadBMP("img/board.bmp");
	if (tempSurface == NULL)
	{
		cout << "Unable to load board image! SDL_Error: " << SDL_GetError() << endl;
		success = false;
	}
	else
	{
		// Create texture from surface pixels
		boardTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
		if (boardTexture == NULL)
		{
			cout << "Unable to create board texture from image! SDL_Error: " << SDL_GetError() << endl;
			success = false;
		}
		else
		{
			// Get rid of unoptimized surface and replace it with the optimized version
			SDL_FreeSurface(tempSurface);
		}
	}

	return success;
}

void GUI::close()
{
	SDL_DestroyTexture(boardTexture);
	boardTexture = NULL;
	
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(renderer);
	renderer = NULL;

	SDL_Quit();
}

SDL_Rect GUI::get_rect(int x, int y) const
{
	SDL_Rect rect;
	rect.x = GUI_XMARGIN + x*GUI_SQWIDTH + GUI_PADDING;
	rect.y = GUI_YMARGIN + y*GUI_SQHEIGHT + GUI_PADDING;
	rect.w = GUI_SQWIDTH - 2 * GUI_PADDING;
	rect.h = GUI_SQHEIGHT - 2 * GUI_PADDING;
	return rect;
}

GUI::GUI(Game *gamep)
{
	game = gamep;
	init();
	loadMedia();
}


GUI::~GUI()
{
	close();
}

void GUI::draw_board(Board_state state)
{
	//cout << "Drawing board" << endl;
	// Draw background
	SDL_RenderCopy(renderer, boardTexture, NULL, NULL);

	int current_player = game->get_current_player();

	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			if (state.board[x][y] != EMPTY)
			{
				SDL_Rect fillRect = get_rect(x, y);
				if (state.board[x][y] == PLAYER1)
				{
					SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
				}
				else if (state.board[x][y] == PLAYER2)
				{
					SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
				}
				//cout << "Drawing a rect x:" << fillRect.x << " y:" << fillRect.y << " w:" << fillRect.w << " h:" << fillRect.h << endl;
				SDL_RenderFillRect(renderer, &fillRect);
			}
			else if (SHOW_LEGAL_MOVES)
			{
				vector<Position> legal_moves = BoardUtil::get_legal_moves(state, current_player);
				for (Position pos : legal_moves)
				{
					if (pos.x == x && pos.y == y)
					{
						SDL_Rect fillRect = get_rect(x, y);
						SDL_SetRenderDrawColor(renderer, 0xFF, 0xCC, 0x00, 0);
						SDL_RenderFillRect(renderer, &fillRect);
						break;
					}
				}
			}
		}
	}

	// Update screen
	SDL_RenderPresent(renderer);
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
