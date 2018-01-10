#pragma once

#define WIDTH 8
#define HEIGHT 8

// Colors
#define C_WHITE 7
#define C_RED 4
#define C_YELLOW 6
#define C_X 32
#define C_O 80
#define C_NUMS 6

#define SHOW_LEGAL_MOVES true

typedef struct
{
	int x, y;
} Position;

typedef struct
{
	int board[WIDTH][HEIGHT];
} Board_state;

typedef struct
{
	int player1;
	int player2;
} Score;