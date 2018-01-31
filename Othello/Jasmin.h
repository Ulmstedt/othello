#pragma once
#include "IPlayer.h"
#include <vector>
#include "Definitions.h"

#define MINIMIZE 0
#define MAXIMIZE 1
#define START_MAX 10

#define HORIZONTAL 0
#define VERTICAL 1

// Positional values
#define V_CORNER 35
#define V_ADJCORNER 1
#define V_DADJCORNER -35
#define V_EDGE 4
#define V_MIDEDGE 3
#define V_TEMPNAME 0

// Situational penalties
#define P_DEADLOCK 30
#define P_EDGERETAKE 30
#define P_BOXIN 30
#define P_OPPCORNER 30

using namespace std;

class Jasmin : public IPlayer
{
	int opponent_id;
	int ai_state;
	float pos_weight[8][8] =
	{
		{ V_CORNER, 1, V_EDGE, V_MIDEDGE, V_MIDEDGE, V_EDGE, 1, V_CORNER },
		{ 1, V_DADJCORNER, 0, -1, -1, 0, V_DADJCORNER, 1 },
		{ V_EDGE, 0, 2, 1, 1, 2, 0, V_EDGE },
		{ V_MIDEDGE, -1, 1, 1, 1, 1, -1, V_MIDEDGE },
		{ V_MIDEDGE, -1, 1, 1, 1, 1, -1, V_MIDEDGE },
		{ V_EDGE, 0, 2, 1, 1, 2, 0, V_EDGE },
		{ 1, V_DADJCORNER, 0, -1, -1, 0, V_DADJCORNER, 1 },
		{ V_CORNER, 1, V_EDGE, V_MIDEDGE, V_MIDEDGE, V_EDGE, 1, V_CORNER }
	};

	void reset_value_grid();
	void compute_min_values(Board_state state, vector<Position> legal_moves);
	void compute_max_values(Board_state state, vector<Position> legal_moves);
	bool on_edge(Position pos) const; // Returns true if pos is an edge position
	void compute_edge_values(Board_state state, vector<Position> legal_moves);
	void check_edge_box_in(Board_state state, Position p, int plane);
	void check_edge_deadlock(Board_state state, Position p, int plane);
	void check_edge_retake(Board_state state, Position p, int plane);
	void guard_corners(Board_state state, vector<Position> legal_moves);
	Position get_best_move(vector<Position> legal_moves) const;
public:
	Jasmin(int id);
	~Jasmin();

	Position play(Board_state) override;
};

