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

// Console colors
#define C_WHITE 7
#define C_RED 4
#define C_YELLOW 6
#define C_X 32
#define C_O 80
#define C_NUMS 6


typedef struct
{
	int x, y;
} Position;

typedef struct
{
	int board[WIDTH][HEIGHT];
	int pieces[2] = { 0, 0 };
} Board_state;


// GUI
#define GUI_XMARGIN 4 // Margin from left side of window
#define GUI_YMARGIN 4 // Margin from top of window
#define GUI_SQWIDTH 64 // 520/8
#define GUI_SQSPACING 1 // Border between squares
#define GUI_SQHEIGHT GUI_SQWIDTH
#define GUI_PADDING 4 // Padding when drawing a piece in a square

#define GUI_MARKERSIZE 8