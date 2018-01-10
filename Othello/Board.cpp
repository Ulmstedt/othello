#include "stdafx.h"
#include "Board.h"
#include <iostream>
#include <stdlib.h>



using namespace std;

Board::Board()
{
	board[3][4] = board[4][3] = 1;
	board[3][3] = board[4][4] = 2;
}


Board::~Board()
{
}

Board_state Board::get_board_state() const
{
	Board_state state;
	memcpy(state.board, board, sizeof(board));
	return state;
}

vector<Position> Board::get_legal_moves(int player) const
{
	vector<Position> positions;
	
	for (int x = 0; x < WIDTH; ++x)
	{
		for (int y = 0; y < HEIGHT; ++y)
		{
			if (board[x][y] == 0)
			{
				Position pos;
				pos.x = x;
				pos.y = y;
				vector<Position> flips = get_flips(pos, player);
				if (flips.size() > 0)
				{
					positions.push_back(pos);
				}
			}
		}
	}

	return positions;
}

bool Board::play(Position pos, int player)
{
	vector<Position> flips = get_flips(pos, player);

	// Illegal move
	if(flips.size() == 0)
		return false;

	board[pos.x][pos.y] = player;
	flip(flips);
}

// Returns a vector containing all the flipped positions from a certain move for a certain player
std::vector<Position> Board::get_flips(Position pos, int player) const
{
	vector<Position> flips;
	if (inside_board(pos) && board[pos.x][pos.y] == 0)
	{
		for (int xdir = -1; xdir <= 1; ++xdir)
		{
			for (int ydir = -1; ydir <= 1; ++ydir)
			{
				vector<Position> temp_flips = check_dir(pos, xdir, ydir, player);
				flips.insert(flips.end(), temp_flips.begin(), temp_flips.end());
			}
		}
	}

	return flips;
}

// Returns the number of flipped pieces from a certain move in a given direction there are
std::vector<Position> Board::check_dir(Position pos, int xdir, int ydir, int player) const
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
		if (!inside_board(next_pos) || board[next_pos.x][next_pos.y] == 0)
		{
			flips.clear();
			return flips;
		}
		else if (board[next_pos.x][next_pos.y] == player)
		{
			return flips;
		}
		else if(board[next_pos.x][next_pos.y] == opponent)
		{
			flips.push_back(next_pos);
		}
	}
}


// Flips all the positions
void Board::flip(vector<Position> positions)
{
	for (Position p : positions)
	{
		board[p.x][p.y] = (board[p.x][p.y] == 1 ? 2 : 1);
	}
}

// Returns true if the position is inside the board
bool Board::inside_board(Position position) const
{
	if (position.x < 0 || position.x > WIDTH - 1 || position.y < 0 || position.y > HEIGHT - 1)
		return false;
	else
		return true;
}




