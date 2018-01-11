#pragma once
#include "IPlayer.h"
#include <SDL.h>
class GUIPlayer : public IPlayer
{
	// Event handler
	SDL_Event e;

public:
	GUIPlayer(int id);
	~GUIPlayer();

	Position play(Board_state board) override;
};

