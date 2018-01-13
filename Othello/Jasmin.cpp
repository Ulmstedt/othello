#include "stdafx.h"
#include "Jasmin.h"
#include <vector>
#include <iostream>
#include "BoardUtil.h"

using namespace std;

void Jasmin::init_pos_weights()
{
	// Corners
	//pos_weight[0][0] = pos_weight[0][7] = pos_weight[7][0] = pos_weight[7][7] = V_CORNER;
	// Positions hor and ver adjacent to corners
	//pos_weight[1][0] = pos_weight[0][1] = pos_weight[6][0] = pos_weight[0][6] =
	//	pos_weight[7][1] = pos_weight[1][7] = pos_weight[6][7] = pos_weight[7][7] = V_ADJCORNER;
	// Positions diagonally adjacent to corners
	//pos_weight[1][1] = pos_weight[6][1] = pos_weight[1][6] = pos_weight[6][6] = V_DADJCORNER;
	// Edges
	//pos_weight[0][2] = pos_weight[2][0] = pos_weight[5][0] = pos_weight[0][5] =
	//	pos_weight[2][7] = pos_weight[7][2] = pos_weight[7][5] = pos_weight[5][7] = V_EDGE;
	// Mid edges
	//pos_weight[3][0] = pos_weight[4][0] = pos_weight[0][3] = pos_weight[0][4] =
	//	pos_weight[3][7] = pos_weight[4][7] = pos_weight[7][3] = pos_weight[7][4] = V_MIDEDGE;

	//pos_weight[2][1] = pos_weight[1][2] = pos_weight[1][5] = pos_weight[2][6] =
	//	pos_weight[5][1] = pos_weight[6][2] = pos_weight[5][6] = pos_weight[6][5] = 0;


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

bool Jasmin::on_edge(Position pos) const
{
	if (pos.x == 0 || pos.x == 7 || pos.y == 0 || pos.y == 7)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Jasmin::compute_edge_values(Board_state state, vector<Position> edge_moves)
{
	for (Position p : edge_moves)
	{
		Board_state temp_state = state;
		temp_state.board[p.x][p.y] = player_id;
		if (p.y == 0 || p.y == 7) // Top and bottom edge
		{
			// ### Check if opponent can take it back immediately
			Position leftpos = { p.x - 1, p.y };
			Position rightpos = { p.x + 1, p.y };
			vector<Position> opp_leftflips = BoardUtil::get_flips(temp_state, leftpos, opponent_id); // Flips if opponent places in leftpos
			vector<Position> opp_rightflips = BoardUtil::get_flips(temp_state, rightpos, opponent_id); // Flips if opponent places in rightpos
			if (opp_leftflips.size() > 0 || opp_rightflips.size() > 0)
			{
				// This is bad, reduce score for that position
				value_grid[p.x][p.y] -= P_EDGERETAKE;
			}

			// ### Check for deadlocks (where there is 1 square free between two own ones)
			leftpos = { p.x - 2, p.y };
			rightpos = { p.x + 2, p.y };
			if ((BoardUtil::inside_board(leftpos) && temp_state.board[leftpos.x][leftpos.y] == player_id && temp_state.board[p.x - 1][p.y] == EMPTY)
				|| (BoardUtil::inside_board(rightpos) && temp_state.board[rightpos.x][rightpos.y] == player_id && temp_state.board[p.x + 1][p.y] == EMPTY))
			{
				// Bad, reduce score
				value_grid[p.x][p.y] -= P_DEADLOCK;
			}
		}
		else if (p.x == 0 || p.x == 7) // Left and right edge
		{
			// ### Check if opponent can take it back immediately
			Position toppos = { p.x, p.y - 1 };
			Position botpos = { p.x, p.y + 1 };
			vector<Position> opp_topflips = BoardUtil::get_flips(temp_state, toppos, opponent_id); // Flips if opponent places in leftpos
			vector<Position> opp_botflips = BoardUtil::get_flips(temp_state, botpos, opponent_id); // Flips if opponent places in rightpos
			if (opp_topflips.size() > 0 || opp_botflips.size() > 0)
			{
				// This is bad, reduce score for that position
				value_grid[p.x][p.y] -= P_EDGERETAKE;
			}

			// ### Check for deadlocks (where there is 1 square free between two own ones)
			toppos = { p.x, p.y - 2 };
			botpos = { p.x, p.y + 2 };
			if ((BoardUtil::inside_board(toppos) && temp_state.board[toppos.x][toppos.y] == player_id && temp_state.board[p.x][p.y - 1] == EMPTY)
				|| (BoardUtil::inside_board(botpos) && temp_state.board[botpos.x][botpos.y] == player_id && temp_state.board[p.x][p.y + 1] == EMPTY))
			{
				// Bad, reduce score
				value_grid[p.x][p.y] -= P_DEADLOCK;
			}
		}
	}

}

Position Jasmin::get_best_move(vector<Position> legal_moves) const
{
	Position move = {0, 0};
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
	opponent_id = (id == PLAYER1 ? PLAYER2 : PLAYER1);
	//init_pos_weights();
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

	// Compute start min/max values
	if (ai_state == MINIMIZE)
	{
		compute_min_values(state, legal_moves);
	}
	else if (ai_state = MAXIMIZE)
	{
		compute_max_values(state, legal_moves);
	}

	// Compute edge values
	vector<Position> edge_moves;
	for (Position p : legal_moves)
	{
		if (on_edge(p))
		{
			edge_moves.push_back(p);
		}
	}
	compute_edge_values(state, edge_moves);



	return get_best_move(legal_moves);
}
