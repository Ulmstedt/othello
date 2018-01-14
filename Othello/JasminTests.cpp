#include "stdafx.h"
#include <iostream>
#include <vector>

#include "Definitions.h"
#include "Board.h"
#include "Jasmin.h"
#include "BoardUtil.h"

void setup_tests();
void run_tests();

using namespace std;

typedef struct
{
	int test_id;
	Board_state state;
	vector<Position> allowed; // Check banned pos only if allowed is empty
	vector<Position> banned;
} JasminTestcase;

vector<JasminTestcase> test_cases;
int num_tests = 0;
int passed = 0;
int failed = 0;


int main2()
{
	setup_tests();
	num_tests = test_cases.size();
	run_tests();
	cout << "--- " << passed << " passed, " << failed << " failed ---" << endl;
	system("PAUSE");
	return 0;
}

void run_tests()
{
	Jasmin *jasmin = new Jasmin(PLAYER2);
	cout << "--- Running " << num_tests << " test(s) for Jasmin ---" << endl;
	for (JasminTestcase jt : test_cases)
	{
		bool test_passed = true;
		Position move = jasmin->play(jt.state);
		// if allowed vector is not empty, check if the pos is matched
		if (jt.allowed.size() > 0)
		{
			bool validpos = false;
			for (Position allowedpos : jt.allowed)
			{
				if (move.x == allowedpos.x && move.y == allowedpos.y)
				{
					validpos = true;
					break;
				}
			}
			if (!validpos) test_passed = false;
		}
		else // If allowed vector is empty, check if position is banned
		{
			for (Position bannedpos : jt.banned)
			{
				if (move.x == bannedpos.x && move.y == bannedpos.y)
				{
					test_passed = false;
				}
			}
		}

		// Increment counters
		if (test_passed)
		{
			passed++;
		}
		else
		{
			cout << "Test " << jt.test_id << " failed! Placed at (" << move.x << ", " << move.y << ")" << endl;
			failed++;
		}
	}

	delete jasmin;

}

void setup_tests()
{
	// Temp variables to push to vectors
	Position banned;
	Position allowed;

	// ### Edge retake situation ###
	JasminTestcase case1;
	case1.test_id = 1;
	// Clear board
	BoardUtil::clear_board(case1.state);
	case1.state.board[0][4] = case1.state.board[1][4] = case1.state.board[2][4] = case1.state.board[3][4] =
		case1.state.board[4][4] = case1.state.board[5][4] = case1.state.board[5][3] = case1.state.board[3][5] =
		case1.state.board[2][6] = case1.state.board[4][6] = case1.state.board[5][5] = PLAYER1;
	case1.state.board[5][2] = case1.state.board[3][3] = case1.state.board[4][3] = case1.state.board[2][5] =
		case1.state.board[4][5] = case1.state.board[3][6] = case1.state.board[5][6] = case1.state.board[2][7] =
		case1.state.board[3][7] = case1.state.board[4][7] = case1.state.board[5][7] = case1.state.board[6][7] = PLAYER2;
	banned = { 0, 3 };
	case1.banned.push_back(banned);
	test_cases.push_back(case1);

	// ### Jasmin makes bad edge move ###
	JasminTestcase case2;
	case2.test_id = 2;
	case2.state.board[1][2] = case2.state.board[0][3] = case2.state.board[2][3] = case2.state.board[2][5] =
		case2.state.board[1][4] = PLAYER1;
	case2.state.board[4][0] = case2.state.board[4][1] = case2.state.board[4][2] = case2.state.board[4][3] =
		case2.state.board[4][4] = case2.state.board[4][5] = case2.state.board[3][2] = case2.state.board[3][3] =
		case2.state.board[3][4] = case2.state.board[3][5] = case2.state.board[2][2] = case2.state.board[2][4] =
		case2.state.board[0][5] = case2.state.board[5][3] = case2.state.board[5][4] = case2.state.board[5][5] =
		case2.state.board[6][4] = case2.state.board[7][5] = PLAYER2;
	banned = { 0, 4 };
	case2.banned.push_back(banned);
	test_cases.push_back(case2);

	// ### Jasmin makes bad edge move ###
	JasminTestcase case3;
	case3.test_id = 3;
	case3.state.board[1][2] = case3.state.board[0][3] = case3.state.board[2][3] = case3.state.board[2][5] =
		case2.state.board[1][4] = PLAYER1;
	case3.state.board[4][0] = case3.state.board[4][1] = case3.state.board[4][2] = case3.state.board[4][3] =
		case3.state.board[4][4] = case3.state.board[4][5] = case3.state.board[3][2] = case3.state.board[3][3] =
		case3.state.board[3][4] = case3.state.board[3][5] = case3.state.board[2][2] = case3.state.board[2][4] =
		case3.state.board[0][5] = case3.state.board[5][3] = case3.state.board[5][4] = case3.state.board[5][5] =
		case3.state.board[6][4] = case3.state.board[7][5] = PLAYER2;
	allowed = { 7, 3 };
	case3.banned.push_back(banned);
	test_cases.push_back(case3);
}
