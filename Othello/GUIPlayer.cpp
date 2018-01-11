#include "stdafx.h"
#include "GUIPlayer.h"
#include "Game.h"
#include "Definitions.h"


GUIPlayer::GUIPlayer(int id)
{
	player_id = id;
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
				move.x = (e.button.x - GUI_XMARGIN) / GUI_SQWIDTH;
				move.y = (e.button.y - GUI_YMARGIN) / GUI_SQHEIGHT;
				return move;
			}
		}
	}
}

