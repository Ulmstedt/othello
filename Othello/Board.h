#pragma once
#include "Definitions.h"
#include <vector>

using namespace std;

class Board
{
	int board[8][8] = { 0 };
	void flip(vector<Position>);

public:
	Board();
	~Board();

	Board_state get_board_state() const;
	bool play(Position, int);
};

