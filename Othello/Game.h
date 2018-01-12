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
	int score[3] = { 0, 0, 0}; // p1, p2, draws

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
	void print_board() const;
	vector<Position> get_move_history() const;
};

