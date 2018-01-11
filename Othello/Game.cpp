#include "stdafx.h"
#include "Game.h"
#include <iostream>
#include "Definitions.h"
#include <windows.h>
#include "BoardUtil.h"

using namespace std;

void Game::next_player()
{
	// Swap current player
	current_player = (current_player == 1 ? 2 : 1);

	Board_state state = board->get_board_state();
	// If new current player has no legal moves, swap back
	if (BoardUtil::get_legal_moves(state, current_player).size() == 0)
	{
		current_player = (current_player == 1 ? 2 : 1);
		// If the other player cant play either, game is finished
		if (BoardUtil::get_legal_moves(state, current_player).size() == 0)
		{
			game_finished = true;
			print_board();
			cout << "Player " << (current_player == 1 ? "X" : "O") << " won! New game? y/n ";
			char ng;
			cin >> ng;
			if (ng == 'y') reset_game();
		}
	}
}

Game::Game(IPlayer *p1, IPlayer *p2)
{
	board = new Board();
	player1 = p1;
	player2 = p2;
	game_finished = false;
	current_player = 1;
	score.player1 = 0;
	score.player2 = 0;
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
	while (!game_finished)
	{
		gameGUI->handle_events();
		
		Board_state state = board->get_board_state();
		gameGUI->draw_board(state);

		Position move;
		if (current_player == 1)
		{
			move = player1->play(state);
		}
		else
		{
			move = player2->play(state);
		}

		bool success = play(move);
		if (success)
		{
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
	current_player = 1;
	move_history.clear();
	game_finished = false;
	
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

Score Game::get_score() const
{
	return score;
}

// Prints the board state to std::out
void Game::print_board() const
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	Board_state state = board->get_board_state();

	system("CLS");

	cout << "  ";
	SetConsoleTextAttribute(hConsole, C_NUMS);
	for (int i = 0; i < WIDTH; ++i)
	{
		cout << " " << i << "  ";
	}

	SetConsoleTextAttribute(hConsole, C_WHITE);
	cout << "\n  -------------------------------\n";

	for (int y = 0; y < HEIGHT; ++y)
	{
		SetConsoleTextAttribute(hConsole, C_NUMS);
		cout << y << " ";
		for (int x = 0; x < WIDTH; ++x)
		{
			switch (state.board[x][y])
			{
			case 0:
				if (SHOW_LEGAL_MOVES)
				{
					vector<Position> legal_moves = BoardUtil::get_legal_moves(state, current_player);
					bool legal = false;
					for (Position pos : legal_moves)
					{
						if (pos.x == x && pos.y == y)
						{
							legal = true;
							SetConsoleTextAttribute(hConsole, C_RED);
							cout << " + ";
							break;
						}
					}
					if (!legal)
					{
						SetConsoleTextAttribute(hConsole, C_WHITE);
						cout << "   ";
					}
				}
				else
				{
					SetConsoleTextAttribute(hConsole, C_WHITE);
					cout << "   ";
				}
				break;
			case 1:
				SetConsoleTextAttribute(hConsole, C_X);
				cout << " X ";
				break;
			case 2:
				SetConsoleTextAttribute(hConsole, C_O);
				cout << " O ";
				break;
			}

			SetConsoleTextAttribute(hConsole, C_WHITE);
			if (x < 7) cout << "|";
		}

		SetConsoleTextAttribute(hConsole, C_WHITE);
		cout << "\n  -------------------------------\n";
	}
}
