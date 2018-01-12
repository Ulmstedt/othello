#pragma once
#include "Definitions.h"
#include <vector>

using namespace std;

class Board
{
	Board_state state;
	void flip(vector<Position>); // Flips all the positions in input vector
	void init_board(int);

public:
	Board();
	~Board();

	Board_state get_board_state() const;
	bool play(Position, int); // Returns success if the move was legal
};

