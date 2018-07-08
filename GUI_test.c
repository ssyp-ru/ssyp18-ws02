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
	actor[1].y = y + 5;
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
	msgs_t * msgs = calloc(1, sizeof(msgs_t));
	msgs->max_size = 1000;
	msgs->buffer = calloc(msgs->max_size, sizeof(msg_t));
	msgs->size = 1;
	msgs->cur = 0;
	msgs->buffer[0].line = "There is nothing here!";
	init_GUI(level->map);
	int input = 0;
	pvector_t * way = calloc(4, sizeof(pvector_t));
	way = find_path((actor[1]), x, y);
	refresh();
	int fx, fy, len = 0;
	fx = 0;
	fy = 0;
	while(input != 'q'){
		bool tick = false;
		render(level->map, actor, features, msgs);
	//	for(int i = 1; i < way->length - 1; i++)
		//	mvaddch(way->buffer[i].y, way->buffer[i].x + 30, '+');
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
			case '[':
				if(msgs->cur > 0)
					msgs->cur--;
					break;
			case ']':
				if(msgs->cur < msgs->size - 1)
					msgs->cur++;
					break;
		}
		refresh();
		if(tick){
			clear();
			way = find_path(actor[1], actor[0].x, actor[0].y);
			fx = way->buffer[0].x;
			fy = way->buffer[0].y;
			len = way->length;
			if(way->length > 0){
				actor[1].x = way->buffer[way[0].length - 1].x;
				actor[1].y = way->buffer[way[0].length - 1].y;				
			}else{
				actor[0].hp--;
				msgs->buffer[msgs->size].line = "A cruel goblin hits you!";
				msgs->size++;
				msgs->cur = msgs->size - 1;
			}
		}
		refresh();
	}
	close_windows();
	endwin();
	return 0;
}
