#pragma once
#include <vector>
#include "Definitions.h"

// Console colors
#define C_WHITE 7
#define C_RED 4
#define C_YELLOW 6
#define C_X 32
#define C_O 80
#define C_NUMS 6

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
	static int get_winner(Board_state); // Returns 0 if black won, 1 if white won or -1 if draw
	static int empty_positions(Board_state);
	static void print_board(Board_state state, int current_player); // Prints board to cout
	static void clear_board(Board_state &state);
};

