#include "game.h"
#include "GUI.h"
#include <ncurses.h>
#include <stdbool.h>
#include "common.h"
#include "levelvector.h"
#include "game.h"
#include "level.h"
#include "actor.h"
#include "feature.h"
#define MAP_SIZE 200

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
	init_colors();	
	start_game();
	endwin();
}

