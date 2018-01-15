#include "stdafx.h"
#include "Jasmin.h"
#include <vector>
#include <iostream>
#include "BoardUtil.h"
#include "GUI.h"
#include <stdlib.h>

using namespace std;


void Jasmin::reset_value_grid()
{
	memcpy(value_grid, pos_weight, sizeof(value_grid));
}

void Jasmin::compute_min_values(Board_state state, vector<Position> legal_moves)
{
	for (Position p : legal_moves)
	{
		// Speculative temp board
		Board_state temp_state = state;

		// Try playing at p
		vector<Position> flips = BoardUtil::get_flips(state, p, player_id);
		temp_state.board[p.x][p.y] = player_id;
		BoardUtil::flip(temp_state, flips);

		// See how many legal moves Jasmin and the opponent would have
		int my_flips = BoardUtil::get_legal_moves(temp_state, player_id).size();
		int opp_flips = BoardUtil::get_legal_moves(temp_state, opponent_id).size();

		// Give points for our own moves and reduce score for opponents moves
		value_grid[p.x][p.y] = value_grid[p.x][p.y] + my_flips - opp_flips;
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
			while (leftpos.x >= 0 && temp_state.board[leftpos.x][leftpos.y] == player_id)
			{
				leftpos.x -= 1;
			}

			Position rightpos = { p.x + 1, p.y };
			while (rightpos.x <= 7 && temp_state.board[rightpos.x][rightpos.y] == player_id)
			{
				rightpos.x += 1;
			}

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
			while (toppos.y >= 0 && temp_state.board[toppos.x][toppos.y] == player_id)
			{
				toppos.y -= 1;
			}

			Position botpos = { p.x, p.y + 1 };
			while (botpos.y <= 7 && temp_state.board[botpos.x][botpos.y] == player_id)
			{
				botpos.y += 1;
			}

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
	vector<Position> best_moves;
	float top_score = -1000;
	for (Position p : legal_moves)
	{
		if (value_grid[p.x][p.y] > top_score)
		{
			top_score = value_grid[p.x][p.y];
			best_moves.clear();
			best_moves.push_back(p);
		}
		else if (value_grid[p.x][p.y] == top_score)
		{
			best_moves.push_back(p);
		}
	}

	if (best_moves.size() == 1)
	{
		return best_moves.front();
	}
	else
	{
		// Select a random move
		int i = rand() % best_moves.size();
		return best_moves.at(i);
	}
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


	//GUI::wait_for_input();
	return get_best_move(legal_moves);
}
