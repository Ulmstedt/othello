#include "stdafx.h"

#include <stdio.h>
#include <iostream>

#include "Game.h"
#include "HumanPlayer.h"
#include "Jasmin.h"


using namespace std;

int main()
{
	IPlayer *player1 = new HumanPlayer();
	IPlayer *player2 = new Jasmin(2);
	Game *game = new Game(player1, player2);
	game->start_game();

    return 0;
}

