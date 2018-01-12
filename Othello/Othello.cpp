#include "stdafx.h"

#include <stdio.h>
#include <iostream>

#include "Game.h"
#include "ConsolePlayer.h"
#include "Jasmin.h"
#include "GUIPlayer.h"
#include "GUI.h"


using namespace std;

int main(int argc, char* args[])
{
	IPlayer *player1 = new GUIPlayer(0);
	IPlayer *player2 = new Jasmin(1);
	Game *game = new Game(player1, player2);
	game->run_game();

    return 0;
}

