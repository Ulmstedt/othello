#include "stdafx.h"

#include <stdio.h>
#include <iostream>

#include "Game.h"
#include "HumanPlayer.h"


using namespace std;

int main()
{
	HumanPlayer *player1 = new HumanPlayer();
	HumanPlayer *player2 = new HumanPlayer();
	Game *game = new Game(player1, player2);
	game->start_game();

    return 0;
}

