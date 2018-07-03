#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdint.h>
#include "graphics.h"
#include "generate.h"

void initCurs(map_t* mapp){
	initscr();
	start_color();
	clearMap(mapp);
	init_pair(1, COLOR_RED, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_GREEN, COLOR_GREEN);
}

void rectangle(map_t* mapp, int x, int x1, int y, int y1){
	UNPACK(map, mapp);
	for (int i = x; i <= x1; i++){
		map[y][i].symbol = '#' | COLOR_PAIR(1);
		map[y1][i].symbol = '#' | COLOR_PAIR(1);
	}
	for (int i = y; i <= y1; i++){
		map[i][x].symbol = '#' | COLOR_PAIR(1);
		map[i][x1].symbol = '#' | COLOR_PAIR(1);
	}
	for(int i = x + 1; i < x1; i++)
		for(int j = y + 1; j < y1; j++)
			map[j][i].symbol = '*';
}
  
void drawSimpleLine(map_t* mapp, int x, int x1, int y, int y1){
	UNPACK(map, mapp);
	for (int i = x; i < x1; i++){
		if (y - 1 >= 0 && map[y - 1][i].symbol == ' ')
			map[y - 1][i].symbol = '#' | COLOR_PAIR(1);
		if (y + 1 < mapp -> width && map[y + 1][i].symbol == ' ')
			map[y + 1][i].symbol = '#' | COLOR_PAIR(1);
		map[y][i].symbol = '*';
	}
	for (int i = y; i < y1; i++){
		if (x1 - 1 >= 0 && map[i][x1 - 1].symbol == ' ')
			map[i][x1 - 1].symbol = '#' | COLOR_PAIR(1);
		if (x1 + 1 < mapp -> height && map[i][x1 + 1].symbol == ' ')
			map[i][x1 + 1].symbol = '#' | COLOR_PAIR(1);
		map[i][x1].symbol = '*';
	}
}

void drawWindow(map_t* mapp){
	UNPACK(map, mapp);
	move(1, 1);
	int j = 0;
	for (int i = 0; i < mapp -> height; i++)
		for (j = 0; j < mapp -> width; j++){
			move(i + 1, j + 1);
			addch(map[j][i].symbol | A_NORMAL);
		}
	refresh();
}
void exitWindow(map_t* mapp){
	UNPACK(map, mapp);
	free(map);
	free(mapp);
	endwin();
}
