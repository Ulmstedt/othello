#pragma once

#define SHOW_LEGAL_MOVES true
#define SHOW_VALUE_GRID_P1 false
#define SHOW_VALUE_GRID_P2 false

#define PLAYER1 0
#define PLAYER2 1
#define EMPTY -1
#define DRAW -1

#define WIDTH 8
#define HEIGHT 8

typedef struct
{
	int x, y;
} Position;

typedef struct
{
	int board[WIDTH][HEIGHT];
	int pieces[2] = { 0, 0 };
} Board_state;

