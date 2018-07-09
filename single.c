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

include <ncurses.h>

void init_colors(){
	start_color();
	init_color(COLOR_MAGENTA, 300, 300,300);
	init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
	init_pair(2, COLOR_RED, COLOR_BLACK);
	init_pair(3, COLOR_BLACK, COLOR_BLUE);
	init_pair(4, COLOR_GREEN, COLOR_BLACK);
}

void start(level_t * level) {
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, true);
	init_GUI();
	init_colors();
		
	actors_vt* actors = level->actors; //init_actors(lvector_get(levels, 0), 10000);
	actors_vt ** queue = calloc(100, sizeof(actors_vt*));
	start_game();
	endwin();
}

