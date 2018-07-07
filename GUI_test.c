#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include "GUI.h"
#include "common.h"
#include "actor.h"
#include "feature.h"

int main(){
	initscr();
	int size_x,size_y;
	int x = 20;
	int y = 20;
	keypad(stdscr,true);
	halfdelay(100);
	curs_set(0);
	actor_t * actor = calloc(1, sizeof(actor_t));
	actor->inventory = calloc(1, sizeof(inventory_t));
	actor->inventory->max_amount = 15;
	actor->inventory->item = 
		calloc(actor->inventory->max_amount, sizeof(item_t));
	actor->inventory->item[0].description = "A small thief knife";
	actor->inventory->item[1].description = "A rotten pie";
	actor->inventory->item[2].description = "A broken helmet";
	actor->inventory->item[0].amount = 1;	
	actor->inventory->item[1].amount = 1;	
	actor->inventory->item[1].flags = FLAG_STACKABLE;
	actor->inventory->item[2].amount = 1;
	actor->inventory->amount = 3;
	actor->hp = 4;
	actor->strength = 10;
	actor->agility = 8;
	actor->stamina = 6;
	actor->symbol = '@';
	actor->x = x;
	actor->y = y;
	feature_t * features = calloc(5, sizeof(feature_t));
	features[0].inventory = calloc(1, sizeof(inventory_t));
	features[0].inventory->item = calloc(10, sizeof(item_t));
	features[1].inventory = calloc(1, sizeof(inventory_t));
	features[1].inventory->item = calloc(10, sizeof(item_t));
	features[0].x = x + 3;
	features[0].y = y;
	features[0].symbol = 'P';
	features[0].inventory->amount = 1;
	features[0].inventory->item[0].description = "A rotten pie";
	features[0].inventory->item[0].flags |= FLAG_STACKABLE;
	features[1].x = x + 5;
	features[1].y = y;
	features[1].symbol = 'Y';
	features[1].inventory->amount = 1;
	features[1].inventory->item[0].description = "A large sword";
	features[1].inventory->item[0].flags &= FLAG_STACKABLE;
	getmaxyx(stdscr, size_y, size_x);
	map_t * _map = calloc(1, sizeof(map_t));
	_map->buffer = calloc(size_x * size_y, sizeof(tile_t));
	_map->width = size_x - 30;
	_map->height = size_y;
	UNPACK(map, _map);
	noecho();
	for(int i = 0; i < size_x; i++){
		for(int k = 0; k < size_y; k++){
			map[k][i].flags &= FLAG_SOLID;
			map[k][i].symbol = '-';
		}
	}
	init_GUI(_map);
	int input = 0;
	while(input != 113){
		render(_map, actor, features);
		input = getch();
		if(input == KEY_RIGHT)
			actor->x++;
		if(input == 112 && features[0].x == actor->x){
			pick_up(actor, features[0]);
		}
		if(input == 112 && features[1].x == actor->x){
			pick_up(actor, features[1]);
		}
		refresh();
	}
	closeWindows();
	endwin();
	return 0;
}
