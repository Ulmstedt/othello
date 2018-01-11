#include "stdafx.h"
#include "ConsolePlayer.h"
#include <iostream>

using namespace std;

ConsolePlayer::ConsolePlayer(int id)
{
	player_id = id;
}


ConsolePlayer::~ConsolePlayer()
{
}

Position ConsolePlayer::play(Board_state board)
{
	Position move;
	cin >> move.x;
	cin >> move.y;

	return move;
}
