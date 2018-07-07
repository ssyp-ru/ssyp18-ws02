#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include "GUI.h"
#include "common.h"
#include "actor.h"
#include "feature.h"
#include "find_path.h"

int main(){
	initscr();
	int size_x,size_y;
	int x = 20;
	int y = 20;
	keypad(stdscr,true);
	halfdelay(100);
	curs_set(0);
	feature_t * features;
	actor_t * actor = calloc(2, sizeof(actor_t));
	actor->inventory = calloc(1, sizeof(inventory_t));
	actor->inventory->max_amount = 15;
	actor->inventory->item = 
		calloc(actor->inventory->max_amount, sizeof(item_t));
	actor[0].hp = 4;
	actor[0].strength = 10;
	actor[0].agility = 8;
	actor[0].stamina = 6;
	actor[0].symbol = '@';
	actor[0].x = x;
	actor[0].y = y;
	actor[1].symbol = 'V';
	actor[1].hp = 2;
	actor[1].x = x + 15;
	actor[1].y = y + 15;
	getmaxyx(stdscr, size_y, size_x);
	map_t * _map = calloc(1, sizeof(map_t));
	_map->buffer = calloc(size_x * size_y, sizeof(tile_t));
	_map->width = size_x - 30;
	_map->height = size_y;
	level_t * level = calloc(1, sizeof(level_t));
	level->map = _map;
	actor[0].level = level;
	actor[1].level = level;
	actor[1].view_radius = 4;
	UNPACK(map, _map);
	noecho();
	for(int i = 0; i < size_x; i++){
		for(int k = 0; k < size_y; k++){
			map[k][i].flags &= FLAG_SOLID;
			map[k][i].symbol = '-';
		}
	}
	init_GUI(level->map);
	int input = 0;
	pvector_t * way;
	way = find_path((actor[1]), x, y);
	while(input != 'q'){
		bool tick = false;
		render(level->map, actor, features);
		for(int i = 0; i < way->length; i++)
			mvaddch(way->buffer[i].y, way->buffer[i].x+30, '+');
		input = getch();
		switch(input){
			case 'w':
				actor[0].y--;
				tick = true;
				break;
			case 'a':
				actor[0].x--;
				tick = true;
				break;
			case 's':
				actor[0].y++;
				tick = true;
				break;
			case 'd':
				actor[0].x++;
				tick = true;
				break;
		}
		if(tick){
			way = find_path(actor[1], actor[0].x, actor[0].y);
			if(way->length > 0){
				actor[1].x = way->buffer[way->length-2].x;
				actor[1].y = way->buffer[way->length-2].y;
			}	
			tick = false;
		}
		refresh();
	}
	close_windows();
	endwin();
	return 0;
}
