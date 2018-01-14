#include "stdafx.h"
#include "GUI.h"
#include <SDL.h>
#include <iostream>
#include "Game.h"
#include "BoardUtil.h"
#include <stdlib.h>

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
			SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

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
	rect.x = GUI_XMARGIN + x*(GUI_SQWIDTH + GUI_SQSPACING) + GUI_PADDING;
	rect.y = GUI_YMARGIN + y*(GUI_SQHEIGHT + GUI_SQSPACING) + GUI_PADDING;
	rect.w = GUI_SQWIDTH - 2 * GUI_PADDING;
	rect.h = GUI_SQHEIGHT - 2 * GUI_PADDING;

	return rect;
}

SDL_Rect GUI::get_marker_rect(int x, int y) const
{
	SDL_Rect rect;
	rect.x = GUI_XMARGIN + x*(GUI_SQWIDTH + GUI_SQSPACING) + GUI_SQWIDTH / 2 - GUI_MARKERSIZE / 2;
	rect.y = GUI_YMARGIN + y*(GUI_SQHEIGHT + GUI_SQSPACING) + GUI_SQHEIGHT / 2 - GUI_MARKERSIZE / 2;
	rect.w = GUI_MARKERSIZE;
	rect.h = GUI_MARKERSIZE;
	
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
	// Draw background
	SDL_RenderCopy(renderer, boardTexture, NULL, NULL);

	int current_player = game->get_current_player();
	vector<Position> legal_moves = BoardUtil::get_legal_moves(state, current_player);

	vector<Position> move_history = game->get_move_history();
	Position last_move;
	if (move_history.size() > 0)
	{
		last_move = move_history.back();
	}

	IPlayer *cur_player = (current_player == PLAYER1 ? game->player1 : game->player2);
	float lowest = 100000, highest = -100000, pval = 0;
	if (current_player == PLAYER1 && SHOW_VALUE_GRID_P1 || current_player == PLAYER2 && SHOW_VALUE_GRID_P2)
	{
		// Find lowest and highest value to adjust color intensity properly
		for (Position p : legal_moves)
		{
			if (cur_player->value_grid[p.x][p.y] < lowest)
			{
				lowest = cur_player->value_grid[p.x][p.y];
			}
			if (cur_player->value_grid[p.x][p.y] > highest)
			{
				highest = cur_player->value_grid[p.x][p.y];
			}
		}
		// Make all values positive to make it easier to calculate alpha properly
		if (lowest < 0)
		{
			highest += abs(lowest) + 1;
			pval += abs(lowest) + 1;
		}
		// Adjust if highest is 0 so that alpha isnt always 0
		if (highest <= 0)
		{
			highest += 1;
			pval += 1;
		}
	}

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
		}
	}

	// Show most recent move
	if (move_history.size() > 0)
	{
		SDL_Rect lmrect = get_marker_rect(last_move.x, last_move.y);
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0x00, 255);
		SDL_RenderFillRect(renderer, &lmrect);
	}

	// Show legal moves
	if (SHOW_LEGAL_MOVES)
	{
		for (Position p : legal_moves)
		{
			SDL_Rect lrect = get_marker_rect(p.x, p.y);
			SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0xFF, 180);
			SDL_RenderFillRect(renderer, &lrect);
		}
	}

	// Show value grid
	if (current_player == PLAYER1 && SHOW_VALUE_GRID_P1 || current_player == PLAYER2 && SHOW_VALUE_GRID_P2)
	{
		for (Position p : legal_moves)
		{
			cout << "(" << p.x << ", " << p.y << "): " << cur_player->value_grid[p.x][p.y] << endl;
			SDL_Rect fillRect = get_rect(p.x, p.y);
			float alpha = (cur_player->value_grid[p.x][p.y] + pval) / highest * 255;
			SDL_SetRenderDrawColor(renderer, alpha, 0, 0, alpha);
			SDL_RenderFillRect(renderer, &fillRect);
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

void GUI::wait_for_input()
{
	SDL_Event e;
	while (1)
	{
		SDL_PollEvent(&e);
		if (e.type == SDL_QUIT)
		{
			Game::quit_game();
		}
		else if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			break;
		}
	}
}
