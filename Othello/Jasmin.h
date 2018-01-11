#pragma once
#include "IPlayer.h"
#include <vector>
#
class Jasmin : public IPlayer
{
public:
	Jasmin(int id);
	~Jasmin();

	Position play(Board_state) override;
};

