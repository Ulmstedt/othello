#pragma once
#include <vector>
#include "Definitions.h"

using namespace std;

class BoardUtil
{
	BoardUtil();
	~BoardUtil();
public:
	static vector<Position> get_legal_moves(Board_state state, int player);
	static vector<Position> check_dir(Board_state state, Position pos, int xdir, int ydir, int player);
	static vector<Position> get_flips(Board_state state, Position pos, int player);
	static bool inside_board(Position);
};

