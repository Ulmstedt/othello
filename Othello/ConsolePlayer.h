#pragma once
#include "IPlayer.h"
class ConsolePlayer : public IPlayer
{
public:
	ConsolePlayer(int id);
	~ConsolePlayer();

	Position play(Board_state board) override;
};

