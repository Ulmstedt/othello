#include "stdafx.h"
#include "Game.h"
#include <iostream>
#include "Definitions.h"
#include <windows.h>

using namespace std;

void Game::next_player()
{
	// Swap current player
	current_player = (current_player == 1 ? 2 : 1);

	// If new current player has no legal moves, swap back
	if (board->get_legal_moves(current_player).size() == 0)
	{
		current_player = (current_player == 1 ? 2 : 1);
		// If the other player cant play either, game is finished
		if (board->get_legal_moves(current_player).size() == 0)
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

Game::Game()
{
	board = new Board();
	game_finished = false;
	current_player = 1;
	score.player1 = 0;
	score.player2 = 0;
}


Game::~Game()
{
	delete board;
}

void Game::start_game()
{
	print_board();
	while (!game_finished)
	{
		cout << "Player " << (current_player == 1 ? "X" : "O") << ": ";

		Position move;
		cin >> move.x;
		cin >> move.y;

		bool success = play(move);
		print_board();
		if (!success) cout << "Illegal move! Try again." << endl;
	}
}

bool Game::play(Position pos)
{
	bool success = board->play(pos, current_player);

	if (success)
	{
		next_player();
		return true;
	}
	else
	{
		return false;
	}
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

vector<Position> Game::get_legal_moves(int player) const
{
	return board->get_legal_moves(player);
}

Score Game::get_score() const
{
	return score;
}

// Prints the board state to std::out
void Game::print_board() const
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	Board_state board_state = board->get_board_state();

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
			switch (board_state.board[x][y])
			{
			case 0:
				if (SHOW_LEGAL_MOVES)
				{
					vector<Position> legal_moves = board->get_legal_moves(current_player);
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