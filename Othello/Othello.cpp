#include "stdafx.h"

#include <stdio.h>
#include <iostream>

#include "Game.h"


using namespace std;

int main()
{
	Game *game = new Game();
	game->start_game();

    return 0;
}

