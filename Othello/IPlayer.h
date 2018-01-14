#pragma once
#include "Definitions.h"
class IPlayer
{
protected:
	int player_id;
public:
	float value_grid[8][8] = { 0 }; // AI:s value grid

	virtual Position play(Board_state) = 0;
	virtual void notify_win(Board_state);
	virtual void notify_loss(Board_state);

};

