#include "game.h"
#include "GUI.h"
#include <ncurses.h>


int main() {
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, true);
	init_GUI();
	start_game();
	endwin();
}

