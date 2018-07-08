#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include "GUI.h"
#include "common.h"
#include "actor.h"
#include "feature.h"
#include "find_path.h"
#include "genmap.h"
#include "time.h"

int main(){
	initscr();
	int size_x,size_y;
	int x = 20;
	int y = 20;
	keypad(stdscr,true);
	halfdelay(100);
	curs_set(0);
	srand(time(NULL));
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
	size_x = 150;
	size_y = 150;
	map_t * _map = create_map(size_x, size_y);
	_map = mapgen_shrew(_map);
	_map = mapgen_rooms_shrew(_map);
	_map = make_walls_shrew(_map);
	level_t * level = calloc(1, sizeof(level_t));
	level->map = _map;
	actor[0].level = level;
	actor[1].level = level;
	actor[1].view_radius = 4;
	noecho();
	/*UNPACK(map, _map);
	for(int i = 0; i < size_x; i++){
		for(int k = 0; k < size_y; k++){
			map[k][i].flags &= FLAG_SOLID;
			map[k][i].symbol = '-';
		}
	}*/
	msgs_t * msgs = calloc(1, sizeof(msgs_t));
	msgs->max_size = 1000;
	msgs->buffer = calloc(msgs->max_size, sizeof(msg_t));
	msgs->size = 1;
	msgs->cur = 0;
	msgs->buffer[0].line = "There is nothing here!";
	box_t box;
	box.x = 0;
	box.y = 0;
	box.width = 70;
	box.height = 37;
	init_GUI(level->map, box);
	int input = 0;
	pvector_t * way = calloc(4, sizeof(pvector_t));
	way = find_path((actor[1]), x, y);
	refresh();
	int fx, fy, len = 0;
	fx = 0;
	fy = 0;
	while(input != 'q'){
		bool tick = false;
		render(level->map, actor, features, msgs, box);
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
		x = actor[0].x;
		y = actor[0].y;
		int x1 = actor[1].x;
		int y1 = actor[1].y;
		if(x > box.width/2 && x < size_x - box.width/2)
			box.x = x - box.width/2;
		if(y > box.height/2 && y < size_y - box.height/2)
			box.y = y - box.height/2;
		way = find_path(actor[1], actor[0].x, actor[0].y);
		refresh();
		if(tick){
			clear();
			fx = way->buffer[0].x;
			fy = way->buffer[0].y;
			len = way->length;
			if(len > 0){
				actor[1].x = way->buffer[len].x;
				actor[1].y = way->buffer[len].y;
			}
			//mvprintw(3,1,"%d  %d", actor[1].x, actor[1].y);
			if(actor[0].x == actor[1].x && actor[0].y == actor[1].y){
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
