#include "stdafx.h"
#include "Board.h"
#include <iostream>
#include <stdlib.h>
#include "Definitions.h"
#include "BoardUtil.h"

using namespace std;

Board::Board()
{
	init_board(0);
}


Board::~Board()
{
}

Board_state Board::get_board_state() const
{
	return state;
}


bool Board::play(Position pos, int player)
{
	vector<Position> flips = BoardUtil::get_flips(state, pos, player);
	
	// Illegal move
	if (flips.size() == 0)
	{
		cout << "Illegal move!\n";
		return false;
	}
		
	state.board[pos.x][pos.y] = player;
	flip(flips);

	// Update pieces counter
	state.pieces[player] += flips.size() + 1;
	int opponent = (player == PLAYER1 ? PLAYER2 : PLAYER1);
	state.pieces[opponent] -= flips.size();
	
	return true;
}


// Flips all the positions in input vector
void Board::flip(vector<Position> positions)
{
	for (Position p : positions)
	{
		state.board[p.x][p.y] = (state.board[p.x][p.y] == PLAYER1 ? PLAYER2 : PLAYER1);
	}
}

void Board::init_board(int b)
{
	// Clear board
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			state.board[x][y] = EMPTY;
		}
	}

	switch (b)
	{
	case 0:
		// Standard board
		state.board[3][4] = state.board[4][3] = PLAYER1;
		state.board[3][3] = state.board[4][4] = PLAYER2;
		state.pieces[0] = state.pieces[1] = 2;
		break;
	case 1:
		// Almost full board to test end of game
		for (int y = 0; y < HEIGHT; ++y)
		{
			for (int x = 0; x < WIDTH; ++x)
			{
				state.board[x][y] = (x + y) % 2;
				state.pieces[(x + y) % 2]++;
			}
		}
		state.board[0][0] = EMPTY;
		state.pieces[0]--;
		break;
	case 2:
		// Edge retake situation
		state.board[0][4] = state.board[1][4] = state.board[2][4] = state.board[3][4] =
			state.board[4][4] = state.board[5][4] = state.board[5][3] = state.board[3][5] =
			state.board[2][6] = state.board[4][6] = state.board[5][5] = PLAYER1;
		state.board[5][2] = state.board[3][3] = state.board[4][3] = state.board[2][5] =
			state.board[4][5] = state.board[3][6] = state.board[5][6] = state.board[2][7] =
			state.board[3][7] = state.board[4][7] = state.board[5][7] = state.board[6][7] = PLAYER2;
		break;
	default:
		break;
	}
}





