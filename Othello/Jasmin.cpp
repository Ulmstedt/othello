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
		// Speculative temp state
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

void Jasmin::compute_edge_values(Board_state state, vector<Position> legal_moves)
{
	for (Position p : legal_moves)
	{
		if (on_edge(p))
		{
			// Speculative temp state
			Board_state temp_state = state;

			// Try playing at p
			vector<Position> flips = BoardUtil::get_flips(state, p, player_id);
			temp_state.board[p.x][p.y] = player_id;
			BoardUtil::flip(temp_state, flips);

			if (p.y == 0 || p.y == 7) // Top and bottom edge
			{
				// Check if opponent can take it back immediately
				check_edge_retake(temp_state, p, HORIZONTAL);

				// Check for deadlocks (where there is 1 square free between two own ones)
				check_edge_deadlock(temp_state, p, HORIZONTAL);

				// Check if the opponent can box himself in between two of our blocking pieces
				check_edge_box_in(temp_state, p, HORIZONTAL);
			}
			else if (p.x == 0 || p.x == 7) // Left and right edge
			{
				// Check if opponent can take it back immediately
				check_edge_retake(temp_state, p, VERTICAL);

				// Check for deadlocks (where there is 1 square free between two own ones)
				check_edge_deadlock(temp_state, p, VERTICAL);

				// Check if the opponent can box himself in between two of our blocking pieces
				check_edge_box_in(temp_state, p, VERTICAL);
			}
		}
	}

}

/*
	O - X O
*/
void Jasmin::check_edge_box_in(Board_state state, Position p, int plane)
{
	Position pos1, pos2;
	if (plane == HORIZONTAL)
	{
		pos1 = { p.x - 1, p.y };
		if (BoardUtil::inside_board(pos1) && state.board[pos1.x][pos1.y] == EMPTY)
		{
			pos1.x -= 1;
			// Move as long as pos2 is opponents piece
			while (BoardUtil::inside_board(pos1) && state.board[pos1.x][pos1.y] == opponent_id)
			{
				pos1.x -= 1;
			}

			if (BoardUtil::inside_board(pos1) && state.board[pos1.x][pos1.y] == player_id)
			{
				// Bad, reduce points
				value_grid[p.x][p.y] -= P_BOXIN;
			}
		}

		pos2 = { p.x + 1, p.y };
		if (BoardUtil::inside_board(pos2) && state.board[pos2.x][pos2.y] == EMPTY)
		{
			pos2.x += 1;
			// Move as long as pos2 is opponents piece
			while (BoardUtil::inside_board(pos2) && state.board[pos2.x][pos2.y] == opponent_id)
			{
				pos2.x += 1;
			}

			if (BoardUtil::inside_board(pos2) && state.board[pos2.x][pos2.y] == player_id)
			{
				// Bad, reduce points
				value_grid[p.x][p.y] -= P_BOXIN;
			}
		}
	}
	else if (plane == VERTICAL)
	{
		pos1 = { p.x, p.y - 1 };
		if (BoardUtil::inside_board(pos1) && state.board[pos1.x][pos1.y] == EMPTY)
		{
			pos1.y -= 1;
			// Move as long as pos2 is opponents piece
			while (BoardUtil::inside_board(pos1) && state.board[pos1.x][pos1.y] == opponent_id)
			{
				pos1.y -= 1;
			}

			if (BoardUtil::inside_board(pos1) && state.board[pos1.x][pos1.y] == player_id)
			{
				// Bad, reduce points
				value_grid[p.x][p.y] -= P_BOXIN;
			}
		}

		pos2 = { p.x, p.y + 1 };
		if (BoardUtil::inside_board(pos2) && state.board[pos2.x][pos2.y] == EMPTY)
		{
			pos2.y += 1;
			// Move as long as pos2 is opponents piece
			while (BoardUtil::inside_board(pos2) && state.board[pos2.x][pos2.y] == opponent_id)
			{
				pos2.y += 1;
			}

			if (BoardUtil::inside_board(pos2) && state.board[pos2.x][pos2.y] == player_id)
			{
				// Bad, reduce points
				value_grid[p.x][p.y] -= P_BOXIN;
			}
		}
	}
}

/*
	O - O
*/
void Jasmin::check_edge_deadlock(Board_state state, Position p, int plane)
{
	Position pos1, pos2;
	if (plane == HORIZONTAL)
	{
		pos1 = { p.x - 2, p.y };
		pos2 = { p.x + 2, p.y };
		if ((BoardUtil::inside_board(pos1) && state.board[pos1.x][pos1.y] == player_id && state.board[p.x - 1][p.y] == EMPTY)
			|| (BoardUtil::inside_board(pos2) && state.board[pos2.x][pos2.y] == player_id && state.board[p.x + 1][p.y] == EMPTY))
		{
			// Bad, reduce score
			value_grid[p.x][p.y] -= P_DEADLOCK;
		}
	}
	else if (plane == VERTICAL)
	{
		pos1 = { p.x, p.y - 2 };
		pos2 = { p.x, p.y + 2 };
		if ((BoardUtil::inside_board(pos1) && state.board[pos1.x][pos1.y] == player_id && state.board[p.x][p.y - 1] == EMPTY)
			|| (BoardUtil::inside_board(pos2) && state.board[pos2.x][pos2.y] == player_id && state.board[p.x][p.y + 1] == EMPTY))
		{
			// Bad, reduce score
			value_grid[p.x][p.y] -= P_DEADLOCK;
		}
	}

}

void Jasmin::check_edge_retake(Board_state state, Position p, int plane)
{
	Position pos1, pos2;
	if (plane == HORIZONTAL)
	{
		pos1 = { p.x - 1, p.y };
		while (pos1.x >= 0 && state.board[pos1.x][pos1.y] == player_id)
		{
			pos1.x -= 1;
		}

		pos2 = { p.x + 1, p.y };
		while (pos2.x <= 7 && state.board[pos2.x][pos2.y] == player_id)
		{
			pos2.x += 1;
		}
	}
	else if (plane == VERTICAL)
	{
		pos1 = { p.x, p.y - 1 };
		while (pos1.y >= 0 && state.board[pos1.x][pos1.y] == player_id)
		{
			pos1.y -= 1;
		}

		pos2 = { p.x, p.y + 1 };
		while (pos2.y <= 7 && state.board[pos2.x][pos2.y] == player_id)
		{
			pos2.y += 1;
		}

	}

	vector<Position> opp_flips1 = BoardUtil::get_flips(state, pos1, opponent_id); // Flips if opponent places in leftpos
	vector<Position> opp_flips2 = BoardUtil::get_flips(state, pos2, opponent_id); // Flips if opponent places in rightpos
	if (opp_flips1.size() > 0 || opp_flips2.size() > 0)
	{
		// This is bad, reduce score for that position
		value_grid[p.x][p.y] -= P_EDGERETAKE;
	}
}

void Jasmin::guard_corners(Board_state state, vector<Position> legal_moves)
{
	for (Position p : legal_moves)
	{
		// Speculative temp state
		Board_state temp_state = state;

		// Try playing at p
		vector<Position> flips = BoardUtil::get_flips(state, p, player_id);
		temp_state.board[p.x][p.y] = player_id;
		BoardUtil::flip(temp_state, flips);

		// Check if opponent can now play a corner
		vector<Position> opp_legal_moves = BoardUtil::get_legal_moves(temp_state, opponent_id);
		for (Position op : opp_legal_moves)
		{
			if (op.x == 0 && op.y == 0
				|| op.x == 7 && op.y == 0
				|| op.x == 0 && op.y == 7
				|| op.x == 7 && op.y == 7)
			{
				cout << "Opponent can take (" << op.x << ", " << op.y << ") if i play (" << p.x << ", " << p.y << ")\n";
				value_grid[p.x][p.y] -= P_OPPCORNER;
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


	compute_edge_values(state, legal_moves);
	guard_corners(state, legal_moves);


	//GUI::wait_for_input();
	return get_best_move(legal_moves);
}
