#pragma once
#include "IPlayer.h"
class HumanPlayer : public IPlayer
{
public:
	HumanPlayer();
	~HumanPlayer();

	Position play(Board_state board) override;
};

