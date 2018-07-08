#include "game.h"
#include <ncurses.h>


int main() {
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, true);
	start_game();
	endwin();
}
