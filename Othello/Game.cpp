#include "stdafx.h"
#include "Game.h"
#include <iostream>
#include "Definitions.h"
#include "BoardUtil.h"
#include <SDL.h>

using namespace std;

void Game::next_player()
{
	cout << "Next player" << endl;
	// Swap current player
	current_player = (current_player == PLAYER1 ? PLAYER2 : PLAYER1);

	Board_state state = board->get_board_state();
	// If new current player has no legal moves, swap back
	if (BoardUtil::get_legal_moves(state, current_player).size() == 0)
	{
		current_player = (current_player == PLAYER1 ? PLAYER2 : PLAYER1);
		// If the other player cant play either, game is finished
		if (BoardUtil::get_legal_moves(state, current_player).size() == 0)
		{
			gameGUI->draw_board(state);
			cout << "Black: " << state.pieces[0] << ", White: " << state.pieces[1] << endl;
			int winner = BoardUtil::get_winner(state);
			if (winner == PLAYER1)
			{
				cout << "Black won!\n";
				score[PLAYER1]++;
				player1->notify_win(state);
				player2->notify_loss(state);
			}
			else if (winner == PLAYER2)
			{
				cout << "White won!\n";
				score[PLAYER2]++;
				player2->notify_win(state);
				player1->notify_loss(state);
			}
			else
			{
				cout << "Draw!\n";
				score[2]++;
			}

			cout << "-- Score --" << endl << "Black: " << score[PLAYER1] << ", White: " << score[PLAYER2] << endl;
			cout << "Click in the window for new game.";
			GUI::wait_for_input();
			cout << "Starting new game\n";
			reset_game();
		}
	}
}

Game::Game(IPlayer *p1, IPlayer *p2)
{
	board = new Board();
	player1 = p1;
	player2 = p2;
	current_player = PLAYER1;
	score[PLAYER1] = score[PLAYER2] = 0;
	gameGUI = new GUI(this);
}


Game::~Game()
{
	delete board;
	delete player1;
	delete player2;
	delete gameGUI;
}

void Game::quit_game()
{
	exit(EXIT_SUCCESS);
}

void Game::run_game()
{
	while (1)
	{
		gameGUI->handle_events();

		Board_state state = board->get_board_state();
		gameGUI->draw_board(state);
		cout << "Black: " << state.pieces[0] << ", White: " << state.pieces[1] << endl;

		Position move;
		if (current_player == PLAYER1)
		{
			cout << "Blacks move\n";
			move = player1->play(state);
		}
		else
		{
			cout << "Whites move\n";
			move = player2->play(state);
		}

		bool success = play(move);
		if (success)
		{
			move_history.push_back(move);
			next_player();
		}
	}
}

bool Game::play(Position pos)
{
	return board->play(pos, current_player);
}

void Game::reset_game()
{
	current_player = PLAYER1;
	move_history.clear();

	delete board;
	board = new Board();
}

Board_state Game::get_board_state() const
{
	return board->get_board_state();
}

int Game::get_current_player() const
{
	return current_player;
}


vector<Position> Game::get_move_history() const
{
	return move_history;
}
