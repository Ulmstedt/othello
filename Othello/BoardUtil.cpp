#include "stdafx.h"
#include "BoardUtil.h"

vector<Position> BoardUtil::get_legal_moves(Board_state state, int player)
{
	vector<Position> positions;

	for (int x = 0; x < WIDTH; ++x)
	{
		for (int y = 0; y < HEIGHT; ++y)
		{
			if (state.board[x][y] == 0)
			{
				Position pos;
				pos.x = x;
				pos.y = y;
				vector<Position> flips = get_flips(state, pos, player);
				if (flips.size() > 0)
				{
					positions.push_back(pos);
				}
			}
		}
	}

	return positions;
}

// Returns a vector containing all the flipped positions from a certain move for a certain player
vector<Position> BoardUtil::get_flips(Board_state state, Position pos, int player)
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
vector<Position> BoardUtil::check_dir(Board_state state, Position pos, int xdir, int ydir, int player)
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
bool BoardUtil::inside_board(Position position)
{
	if (position.x < 0 || position.x > WIDTH - 1 || position.y < 0 || position.y > HEIGHT - 1)
		return false;
	else
		return true;
}