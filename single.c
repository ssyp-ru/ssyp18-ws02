#include "game.h"
#include "common.h"
#include "level.h"
#include "actor.h"
#include "roomvector.h"
#include <ncurses.h>
#include "feature_prototypes.h"

void init_colors(){
	start_color();
	init_color(COLOR_MAGENTA, 300, 300,300);
	init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_BLACK, COLOR_BLUE);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);
}

int main() {
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, true);
	init_GUI();
	init_protofeatures();
	init_colors();
	start_game();
	free_protofeatures();
	endwin();
}
