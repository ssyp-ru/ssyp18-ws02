#include "game.h"
#include "GUI.h"
#include <ncurses.h>

void init_colors(){
start_color();
init_color(COLOR_MAGENTA, 300, 300,300);
init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
init_pair(2, COLOR_RED, COLOR_BLACK);
init_pair(3, COLOR_BLACK, COLOR_BLUE);
}

int main() {
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, true);
	init_GUI();
	init_colors();
	start_game();
	endwin();
}

