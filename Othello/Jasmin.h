#pragma once
#include "IPlayer.h"
#include <vector>
#include "Definitions.h"

#define MINIMIZE 0
#define MAXIMIZE 1

#define V_CORNER 15
#define V_ADJCORNER 1
#define V_DADJCORNER -10
#define V_EDGE 4
#define V_MIDEDGE 3
#define V_TEMPNAME 0

using namespace std;

class Jasmin : public IPlayer
{
	int ai_state;
	float pos_weight[8][8] = { 0 };
	float value_grid[8][8];

	void init_pos_weights();
	void reset_value_grid();
	void compute_min_values(Board_state state, vector<Position> legal_moves);
	void compute_max_values(Board_state state, vector<Position> legal_moves);
	Position get_best_move(vector<Position> legal_moves) const;
public:
	Jasmin(int id);
	~Jasmin();

	Position play(Board_state) override;
};

