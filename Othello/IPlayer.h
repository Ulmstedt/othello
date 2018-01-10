#pragma once
#include "Definitions.h"
class IPlayer
{
public:
	virtual Position play(Board_state) = 0;
};

