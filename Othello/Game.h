#pragma once
#include <vector>
#include "Definitions.h"
#include "Board.h"
#include "IPlayer.h"

using namespace std;

class Game
{
	Board *board;
	int current_player;
	Score score;
	bool game_finished;
	IPlayer *player1;
	IPlayer *player2;


	vector<Position> move_history;
	void next_player();

public:
	Game(IPlayer *player1, IPlayer *player2);
	~Game();

	void start_game();
	bool play(Position);
	void reset_game();
	Board_state get_board_state() const;
	int get_current_player() const;
	vector<Position> get_legal_moves(int) const;
	Score get_score() const;
	void print_board() const;
};

