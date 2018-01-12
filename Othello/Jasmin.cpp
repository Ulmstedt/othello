#include "stdafx.h"
#include "Jasmin.h"
#include <vector>
#include <iostream>
#include "BoardUtil.h"

using namespace std;

void Jasmin::init_pos_weights()
{
	// Corners
	pos_weight[0][0] = pos_weight[0][7] = pos_weight[7][0] = pos_weight[7][7] = V_CORNER;
	// Positions hor and ver adjacent to corners
	pos_weight[1][0] = pos_weight[0][1] = pos_weight[6][0] = pos_weight[0][6] =
		pos_weight[7][1] = pos_weight[1][7] = pos_weight[6][7] = pos_weight[7][7] = V_ADJCORNER;
	// Positions diagonally adjacent to corners
	pos_weight[1][1] = pos_weight[6][1] = pos_weight[1][6] = pos_weight[6][6] = V_DADJCORNER;
	// Edges
	for (int i = 2; i <= 5; ++i)
	{
		pos_weight[i][0] = pos_weight[0][i] = pos_weight[i][7] = pos_weight[7][i] = V_EDGE;
	}

}

void Jasmin::reset_value_grid()
{
	memcpy(value_grid, pos_weight, sizeof(value_grid));
}

void Jasmin::compute_min_values(Board_state state, vector<Position> legal_moves)
{
	for (Position p : legal_moves)
	{
		vector<Position> flips = BoardUtil::get_flips(state, p, player_id);
		value_grid[p.x][p.y] -= flips.size();
	}
}

void Jasmin::compute_max_values(Board_state state, vector<Position> legal_moves)
{
	for (Position p : legal_moves)
	{
		vector<Position> flips = BoardUtil::get_flips(state, p, player_id);
		value_grid[p.x][p.y] += flips.size();
	}
}

Position Jasmin::get_best_move(vector<Position> legal_moves) const
{
	Position move;
	float top_score = -1000;
	for (Position p : legal_moves)
	{
		if (value_grid[p.x][p.y] > top_score)
		{
			move = p;
			top_score = value_grid[p.x][p.y];
		}
	}

	return move;
}

Jasmin::Jasmin(int id)
{
	player_id = id;
	init_pos_weights();
	ai_state = MINIMIZE;
}


Jasmin::~Jasmin()
{
}

Position Jasmin::play(Board_state state)
{
	if (ai_state == MINIMIZE)
	{
		if (BoardUtil::empty_positions(state) <= 12)
		{
			ai_state = MAXIMIZE;
		}
	}

	reset_value_grid();

	vector<Position> legal_moves = BoardUtil::get_legal_moves(state, player_id);

	if (ai_state == MINIMIZE)
	{
		compute_min_values(state, legal_moves);
	}
	else if (ai_state = MAXIMIZE)
	{
		compute_max_values(state, legal_moves);
	}
	return get_best_move(legal_moves);
}
