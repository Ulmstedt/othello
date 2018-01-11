#include "stdafx.h"
#include "GUIPlayer.h"
#include "Game.h"

#define XCLICK_MARGIN 4
#define YCLICK_MARGIN 4
#define XCLICK_DIV 65 // 520/8
#define YCLICK_DIV XCLICK_DIV

GUIPlayer::GUIPlayer(int id)
{
	player_id = id;
}

GUIPlayer::~GUIPlayer()
{
}

Position GUIPlayer::play(Board_state board)
{
	Position move;

	while (1)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			// Quit event
			if (e.type == SDL_QUIT)
			{
				Game::quit_game();
			}
			else if (e.type == SDL_MOUSEBUTTONDOWN)
			{
				//cout << "Pressed (" << e.button.x << ", " << e.button.y << ")\n";
				move.x = (e.button.x - XCLICK_MARGIN) / XCLICK_DIV;
				move.y = (e.button.y - YCLICK_MARGIN) / YCLICK_DIV;
				return move;
			}
		}
	}
}

