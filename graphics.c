#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include "graphics.h"
#include "generate.h"

void initCurs(map_t* mapp){
	initscr();
	halfdelay(1);
	noecho();
	curs_set(0);
	start_color();
	clearMap(mapp);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_GREEN);
}

void rectangle(map_t* mapp, int x, int x1, int y, int y1){
	UNPACK(map, mapp);
	for (int i = x; i <= x1; i++){
		map[i][y].symbol = '#' | COLOR_PAIR(1);
		map[i][y1].symbol = '#' | COLOR_PAIR(1);
	}
	for (int i = y; i <= y1; i++){
		map[x][i].symbol = '#' | COLOR_PAIR(1);
		map[x1][i].symbol = '#' | COLOR_PAIR(1);
	}
	for(int i = x + 1; i < x1; i++){
		for(int j = y + 1; j < y1; j++){
			map[i][j].symbol = '*';
		}
	}
}
  
void drawSimpleLine(map_t* mapp, int x, int x1, int y, int y1){
	UNPACK(map, mapp);
	for (int i = x; i < x1; i++){
		if (y - 1 >= 0 && map[i][y - 1].symbol == ' '){
			map[i][y - 1].symbol = '#' | COLOR_PAIR(1);
		}
		if (y + 1 < mapp->width && map[i][y + 1].symbol == ' '){
			map[i][y + 1].symbol = '#' | COLOR_PAIR(1);
		}
		map[i][y].symbol = '*';
	}
	for (int i = y; i < y1; i++){
		if (x1 - 1 >= 0 && map[x1 - 1][i].symbol == ' '){
			map[x1 - 1][i].symbol = '#' | COLOR_PAIR(1);
		}
		if (x1 + 1 < mapp->height && map[x1 + 1][i].symbol == ' '){
			map[x1 + 1][i].symbol = '#' | COLOR_PAIR(1);
		}
		map[x1][i].symbol = '*';
	}
}

void drawWindow(map_t* mapp){
	UNPACK(map, mapp);
	int j = 0;
	for (int i = 0; i < mapp->height; i++){
		for (j = 0; j < mapp->width; j++){
			move(i + 3, j + 1);
			addch(map[i][j].symbol | A_NORMAL);
		}
	}
	refresh();
}
void exitWindow(map_t* mapp){
	UNPACK(map, mapp);
	free(map);
	free(mapp);
	endwin();
}
