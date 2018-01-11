#pragma once
#include <vector>
#include "Definitions.h"
#include "Board.h"
#include "IPlayer.h"
#include "GUI.h"

using namespace std;

class Game
{
	Board *board = NULL;
	int current_player;
	Score score;
	bool game_finished = false;
	bool quit = false;

	IPlayer *player1 = NULL;
	IPlayer *player2 = NULL;

	GUI *gameGUI = NULL;

	vector<Position> move_history;
	void next_player();

public:
	Game(IPlayer *player1, IPlayer *player2);
	~Game();

	void run_game();
	bool play(Position);
	void reset_game();
	Board_state get_board_state() const;
	int get_current_player() const;
	vector<Position> get_legal_moves(int) const;
	Score get_score() const;
	void print_board() const;
};

