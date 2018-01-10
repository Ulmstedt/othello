#pragma once
#include "Definitions.h"
#include <vector>

using namespace std;

class Board
{
	int board[8][8] = { 0 };
	void flip(vector<Position>);
	vector<Position> check_dir(Position, int, int, int) const;

public:
	Board();
	~Board();

	Board_state get_board_state() const;
	vector<Position> get_legal_moves(int) const;
	vector<Position> get_flips(Position, int) const;
	bool play(Position, int);
	bool inside_board(Position) const;
};

