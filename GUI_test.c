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
<<<<<<< HEAD

#define MAP_SIZE 1000

// XXX XXX XXX XXX XXX XXX XXX XXX
//  This file will be deleted ASAP
// XXX XXX XXX XXX XXX XXX XXX XXX
=======
>>>>>>> Dasha

int main(){
	initscr();
	int size_x, size_y;
	int x = 10;
	int y = 10;
	keypad(stdscr, true);
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
	actor[0].x = MAP_SIZE-10;
	actor[0].y = MAP_SIZE-10;
	actor[1].symbol = 'V';
	actor[1].hp = 2;
	actor[1].x = x + 15;
	actor[1].y = y + 5;
	getmaxyx(stdscr, size_y, size_x);
	size_x = MAP_SIZE;
	size_y = MAP_SIZE;
	map_t * _map = gen_map(size_x, size_y);
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
	msgs_t * msgs = calloc(1, sizeof(msgs_t));
	msgs->max_size = 1000;
	msgs->buffer = calloc(msgs->max_size, sizeof(msg_t));
	msgs->size = 1;
	msgs->cur = 0;
	msgs->buffer[0].line = "There is nothing here!";
	init_GUI(level->map, box);
	int input = 0;
	pvector_t * way = calloc(4, sizeof(pvector_t));
	way = find_path((actor[1]), x, y);
	way->buffer = calloc(MAP_SIZE*2,sizeof(coord_t));
	refresh();
	int fx, fy, len = 0;
	fx = 0;
	fy = 0;
	char str[9999] = {0};
	FILE * file;
	file = fopen("loss.txt", "r");
	while(input != 'q'){
		bool tick = false;
		if(actor[0].hp > 0)
			render(level->map, actor, features, msgs, box);
		else if(file){
			move(0,0);
			while(!feof(file)) {	
				if(!feof(file))
					printw("%s", str);
				fgets(str,100, file);
			} 
			fclose(file);
			getch();
			break;
		}
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
