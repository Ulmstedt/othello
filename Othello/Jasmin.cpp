#include "stdafx.h"
#include "Jasmin.h"
#include <vector>
#include "Definitions.h"
#include <iostream>
#include "BoardUtil.h"

using namespace std;

Jasmin::Jasmin(int id)
{
	player_id = id;
}


Jasmin::~Jasmin()
{
}

Position Jasmin::play(Board_state state)
{
	Position move;
	move.x = 0;
	move.y = 0;
	int min_flips = 100;
	for (int y = 0; y < HEIGHT; ++y) 
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			Position temp_move;
			temp_move.x = x;
			temp_move.y = y;
			vector<Position> flips = BoardUtil::get_flips(state, temp_move, player_id);
			//cout << "(" << temp_move.x << ", " << temp_move.y << ") flips " << flips.size() << endl; 
			if (flips.size() > 0 && flips.size() < min_flips)
			{
				//cout << "Changing to (" << temp_move.x << ", " << temp_move.y << ")" << endl;
				move = temp_move;
				min_flips = flips.size();
			}
		}
	}
	//cout << "Jasmin plays (" << move.x << ", " << move.y << ")\n";
	//system("PAUSE");
	return move;
}
