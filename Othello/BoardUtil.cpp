#include "stdafx.h"
#include "BoardUtil.h"
#include <windows.h>

vector<Position> BoardUtil::get_legal_moves(Board_state state, int player)
{
	vector<Position> positions;

	for (int x = 0; x < WIDTH; ++x)
	{
		for (int y = 0; y < HEIGHT; ++y)
		{
			if (state.board[x][y] == EMPTY)
			{
				Position pos;
				pos.x = x;
				pos.y = y;
				vector<Position> flips = get_flips(state, pos, player);
				if (flips.size() > 0)
				{
					positions.push_back(pos);
				}
			}
		}
	}

	return positions;
}

// Returns a vector containing all the flipped positions from a certain move for a certain player
vector<Position> BoardUtil::get_flips(Board_state state, Position pos, int player)
{
	vector<Position> flips;
	if (inside_board(pos) && state.board[pos.x][pos.y] == EMPTY)
	{
		for (int xdir = -1; xdir <= 1; ++xdir)
		{
			for (int ydir = -1; ydir <= 1; ++ydir)
			{
				vector<Position> temp_flips = check_dir(state, pos, xdir, ydir, player);
				flips.insert(flips.end(), temp_flips.begin(), temp_flips.end());
			}
		}
	}

	return flips;
}

// Returns the number of flipped pieces from a certain move in a given direction there are
vector<Position> BoardUtil::check_dir(Board_state state, Position pos, int xdir, int ydir, int player)
{
	vector<Position> flips;
	int opponent = (player == PLAYER1 ? PLAYER2 : PLAYER1);

	Position next_pos;
	next_pos.x = pos.x;
	next_pos.y = pos.y;

	while (1)
	{
		next_pos.x += xdir;
		next_pos.y += ydir;
		if (!inside_board(next_pos) || state.board[next_pos.x][next_pos.y] == EMPTY)
		{
			flips.clear();
			return flips;
		}
		else if (state.board[next_pos.x][next_pos.y] == player)
		{
			return flips;
		}
		else if (state.board[next_pos.x][next_pos.y] == opponent)
		{
			flips.push_back(next_pos);
		}
	}
}

// Returns true if the position is inside the board
bool BoardUtil::inside_board(Position position)
{
	if (position.x < 0 || position.x > WIDTH - 1 || position.y < 0 || position.y > HEIGHT - 1)
		return false;
	else
		return true;
}

int BoardUtil::get_winner(Board_state state)
{
	if (state.pieces[0] > state.pieces[1])
	{
		return PLAYER1;
	}
	else if (state.pieces[1] > state.pieces[0])
	{
		return PLAYER2;
	}
	else
	{
		return DRAW;
	}
}

int BoardUtil::empty_positions(Board_state state)
{
	int empty_positions = 0;
	for (int x = 0; x < WIDTH; ++x)
	{
		for (int y = 0; y < HEIGHT; ++y)
		{
			if (state.board[x][y] == EMPTY) empty_positions++;
		}
	}

	return empty_positions;
}

// Prints the board state to std::out
void BoardUtil::print_board(Board_state state, int current_player)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

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
			case EMPTY:
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
			case PLAYER1:
				SetConsoleTextAttribute(hConsole, C_X);
				cout << " X ";
				break;
			case PLAYER2:
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