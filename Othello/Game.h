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

	IPlayer *player1 = NULL;
	IPlayer *player2 = NULL;

	GUI *gameGUI = NULL;

	vector<Position> move_history;
	void next_player();
	bool play(Position);

public:
	Game(IPlayer *player1, IPlayer *player2);
	~Game();

	static void quit_game();

	void run_game();
	void reset_game();
	Board_state get_board_state() const;
	int get_current_player() const;
	Score get_score() const;
	void print_board() const;
};

