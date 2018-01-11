#pragma once

#define WIDTH 8
#define HEIGHT 8

// Console colors
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


// GUI
#define GUI_XMARGIN 4 // Margin from left side of window
#define GUI_YMARGIN 4 // Margin from top of window
#define GUI_SQWIDTH 65 // 520/8
#define GUI_SQHEIGHT GUI_SQWIDTH
#define GUI_PADDING 4 // Padding when drawing a piece in a square