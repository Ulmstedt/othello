#include "stdafx.h"
#include "Jasmin.h"
#include <vector>
#include "Definitions.h"
#include <iostream>

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
			vector<Position> flips = get_flips(state, temp_move, player_id);
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

// Returns a vector containing all the flipped positions from a certain move for a certain player
vector<Position> Jasmin::get_flips(Board_state state, Position pos, int player) const
{
	vector<Position> flips;
	if (inside_board(pos) && state.board[pos.x][pos.y] == 0)
	{
		for (int xdir = -1; xdir <= 1; ++xdir)
		{
			for (int ydir = -1; ydir <= 1; ++ydir)
			{
				vector<Position> temp_flips = check_dir(state, pos, xdir, ydir, player);
				flips.insert(flips.end(), temp_flips.begin(), temp_flips.end());
			}
		}
	}

	return flips;
}

// Returns the number of flipped pieces from a certain move in a given direction there are
vector<Position> Jasmin::check_dir(Board_state state, Position pos, int xdir, int ydir, int player) const
{
	vector<Position> flips;
	int opponent = (player == 1 ? 2 : 1);

	Position next_pos;
	next_pos.x = pos.x;
	next_pos.y = pos.y;

	while (1)
	{
		next_pos.x += xdir;
		next_pos.y += ydir;
		if (!inside_board(next_pos) || state.board[next_pos.x][next_pos.y] == 0)
		{
			flips.clear();
			return flips;
		}
		else if (state.board[next_pos.x][next_pos.y] == player)
		{
			return flips;
		}
		else if (state.board[next_pos.x][next_pos.y] == opponent)
		{
			flips.push_back(next_pos);
		}
	}
}


// Returns true if the position is inside the board
bool Jasmin::inside_board(Position position) const
{
	if (position.x < 0 || position.x > WIDTH - 1 || position.y < 0 || position.y > HEIGHT - 1)
		return false;
	else
		return true;
}