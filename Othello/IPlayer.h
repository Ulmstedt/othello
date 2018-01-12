#pragma once
#include "Definitions.h"
class IPlayer
{
protected:
	int player_id;
public:
	virtual Position play(Board_state) = 0;
	virtual void notify_win();
	virtual void notify_loss();
};

