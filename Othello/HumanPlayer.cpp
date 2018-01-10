#include "stdafx.h"
#include "HumanPlayer.h"
#include <iostream>

using namespace std;

HumanPlayer::HumanPlayer()
{
}


HumanPlayer::~HumanPlayer()
{
}

Position HumanPlayer::play(Board_state board)
{
	Position move;
	cin >> move.x;
	cin >> move.y;

	return move;
}
