#pragma once
#include "IPlayer.h"
#include <vector>
#include "Definitions.h"

#define MINIMIZE 0
#define MAXIMIZE 1

// Positional values
#define V_CORNER 15
#define V_ADJCORNER 1
#define V_DADJCORNER -10
#define V_EDGE 4
#define V_MIDEDGE 3
#define V_TEMPNAME 0

// Situational penalties
#define P_DEADLOCK 25
#define P_EDGERETAKE 25

using namespace std;

class Jasmin : public IPlayer
{
	int opponent_id;
	int ai_state;
	float pos_weight[8][8] =
	{
		{ V_CORNER, 1, 4, 3, 3, 4, 1, V_CORNER },
		{ 1, -10, 0, -1, -1, 0, -10, 1 },
		{ 4, 0, 2, 1, 1, 2, 0, 4 },
		{ 3, -1, 1, 1, 1, 1, -1, 3 },
		{ 3, -1, 1, 1, 1, 1, -1, 3 },
		{ 4, 0, 2, 1, 1, 2, 0, 4 },
		{ 1, -10, 0, -1, -1, 0, -10, 1 },
		{ V_CORNER, 1, 4, 3, 3, 4, 1, V_CORNER }
	};
	float value_grid[8][8];

	void init_pos_weights();
	void reset_value_grid();
	void compute_min_values(Board_state state, vector<Position> legal_moves);
	void compute_max_values(Board_state state, vector<Position> legal_moves);
	bool on_edge(Position pos) const; // Returns true if pos is an edge position
	void compute_edge_values(Board_state state, vector<Position> edge_moves);
	Position get_best_move(vector<Position> legal_moves) const;
public:
	Jasmin(int id);
	~Jasmin();

	Position play(Board_state) override;
};

