#include "stdafx.h"
#include "Board.h"
#include <iostream>
#include <stdlib.h>
#include "Definitions.h"
#include "BoardUtil.h"

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


bool Board::play(Position pos, int player)
{
	Board_state state = get_board_state();
	vector<Position> flips = BoardUtil::get_flips(state, pos, player);

	// Illegal move
	if(flips.size() == 0)
		return false;

	board[pos.x][pos.y] = player;
	flip(flips);
}


// Flips all the positions in input vector
void Board::flip(vector<Position> positions)
{
	for (Position p : positions)
	{
		board[p.x][p.y] = (board[p.x][p.y] == 1 ? 2 : 1);
	}
}





