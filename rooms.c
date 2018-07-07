#include <math.h>
#include "common.h"
#include "rooms.h"
#include <stdlib.h>
#include <ncurses.h>
#include <string.h>
#include "dumblogger.h"
void room_free(room_t* room) {
	if(room != NULL) free(room);
}
void create_line (int x1, int y1, int x2, int y2, char ch, int COLOR, map_t* map) {
	UNPACK(field, map);
	double x=x1, y=y1;
	if (x1-x2 == 0) {
		double l = 0;
		double c = x1;
		while (y != y2) {
			x = l*y + c;
			field[(int)y][(int)x].symbol = ch | COLOR_PAIR(COLOR);
			if (y2 >= y) {
				x++;
				y++;
			} else {
				x--;
				y--;
			}
		}
	} else {
		double k = (y1-y2)/(x1-x2);
		double b = y1 - k * x1;
		while (true) {
			if (x2 - x > y2 - y || y1 - y2 == 0) {
				y = k*x + b;
				x = round(x);
				y = round(y);
				field[(int)y][(int)x].symbol = ch | COLOR_PAIR(COLOR);
				if (x2 >= x) {
					if (x >= x2) break;
					x++;
					y++;
				} else {
					if (x <= x2) break;
					x--;
					y--;
				}
			} else {
				x = (y - b)/k;
				x = round(x);
				y = round(y);
				field[(int)y][(int)x].symbol = ch | COLOR_PAIR(COLOR);
				if (y2 >= y) {
					if (y >= y2) break;
					x++;
					y++;
				} else {
					if (y <= y2) break;
					x--;
					y--;
				}
			}
		}
	}
}
void div_room (tree_t* parent, vector_t* leaves, map_t* map) {
	room_t* parent_room = parent->value;
	if(parent_room->S <= 64 || (parent_room->len_x <=16 && parent_room->len_y <= 8)) {
		vector_add(leaves, parent);
		return;
	}
	room_t* next_rooms[2];
	next_rooms[0] = malloc(sizeof(room_t));
	next_rooms[1] = malloc(sizeof(room_t));
	int len_last;
	int x1=0, y1=0, x2=0, y2=0;
	if (parent_room->len_x > parent_room->len_y*2) {
		len_last = parent_room->len_x;
		x1 = parent_room->x1 + (rand() %((len_last*40)/100) + ((len_last*30)/100));
		y1 = parent_room->y1;
		x2 = x1;
		y2 = parent_room->y2;
		create_line(x1, y1, x2, y2, '#', 1, map);
		next_rooms[1]->x2 = parent_room->x2;
		next_rooms[1]->y2 = parent_room->y2;
		next_rooms[1]->x1 = x2;
		next_rooms[1]->y1 = y1;
		next_rooms[0]->x2 = x2;
		next_rooms[0]->y2 = y2;
		next_rooms[0]->x1 = parent_room->x1;
		next_rooms[0]->y1 = parent_room->y1;
	} else {
		len_last = parent_room->len_y;
		y2 = parent_room->y2 + (rand() %((len_last*40)/100) + ((len_last*30)/100));
		x2 = parent_room->x2;
		y1 = y2;
		x1 = x2 - parent_room->len_x;
		create_line(x1, y1, x2, y2, '#', 1, map);
		next_rooms[1]->x2 = parent_room->x2;
		next_rooms[1]->y2 = parent_room->y2;
		next_rooms[1]->x1 = parent_room->x1;
		next_rooms[1]->y1 = y2;
		next_rooms[0]->x2 = x2;
		next_rooms[0]->y2 = y2;
		next_rooms[0]->x1 = parent_room->x1;
		next_rooms[0]->y1 = parent_room->y1;
	}
	next_rooms[0]->len_x = next_rooms[0]->x2 - next_rooms[0]->x1;
	next_rooms[0]->len_y = next_rooms[0]->y1 - next_rooms[0]->y2;
	next_rooms[0]->S = next_rooms[0]->len_x * next_rooms[0]->len_y;
	next_rooms[1]->len_x = next_rooms[1]->x2 - next_rooms[1]->x1;
	next_rooms[1]->len_y = next_rooms[1]->y1 - next_rooms[1]->y2;
	next_rooms[1]->S = next_rooms[1]->len_x * next_rooms[1]->len_y;

	for (int i=0; i< 2; i++) {
		tree_add(parent, next_rooms[i]);
	}
	div_room(parent->right, leaves, map);
	div_room(parent->left, leaves, map);
}
//В данной функции parent_room является левой или правой комнатой относительно room
void div_walls_of_rooms (tree_t*	leaves, map_t* map) {
	UNPACK(field, map);
	if (leaves->parent != NULL && leaves != NULL) {
		room_t* room = leaves->value;
		room_t* parent_room;
		if (leaves->parent->right->value == leaves->value) {
			parent_room = leaves->parent->left->value;
		} else {
			parent_room = leaves->parent->right->value;
		}
		if (parent_room->x1 > room->x1) {
			room->door_x[0] = parent_room->x1;
			room->door_x[1] = parent_room->x1;
			room->door_y[0] = room->y1 + (parent_room->y2 - room->y1)/2;
			room->door_y[1] = room->y1 + (parent_room->y2 - room->y1)/2 - 1;	
			while ((char)field[room->door_y[0]][room->door_x[0]+1].symbol  == '#' ||
				(char)field[room->door_y[0]][room->door_x[0]-1].symbol == '#' ||
				(char)field[room->door_y[1]][room->door_x[1]+1].symbol  == '#' ||
				(char)field[room->door_y[1]][room->door_x[1]-1].symbol == '#') {
				room->door_y[0]++;
				room->door_y[1]++;			
			}
		}
		else if (parent_room->x1 < room->x1) {
			room->door_x[0] = room->x1;
			room->door_x[1] = room->x1;
			room->door_y[0] = room->y1 + (parent_room->y2 - room->y1)/2;
			room->door_y[1] = room->y1 + (parent_room->y2 - room->y1)/2 - 1;	
			while ((char)field[room->door_y[0]][room->door_x[0]+1].symbol  == '#' ||
				(char)field[room->door_y[0]][room->door_x[0]-1].symbol == '#' ||
				(char)field[room->door_y[1]][room->door_x[1]+1].symbol  == '#' ||
				(char)field[room->door_y[1]][room->door_x[1]-1].symbol == '#') {
				room->door_y[0]++;
				room->door_y[1]++;
			}
		}
		else if (parent_room->y1 < room->y1) {
			room->door_x[0] = room->x1 + (parent_room->x2 - room->x1)/2;
		 	room->door_x[1] = room->x1 + (parent_room->x2 - room->x1)/2 - 1;		
			room->door_y[0] = parent_room->y1;
			room->door_y[1] = parent_room->y1;
			while ((char)field[room->door_y[0]+1][room->door_x[0]].symbol  == '#' ||
				(char)field[room->door_y[0]-1][room->door_x[0]].symbol  == '#' ||
				(char)field[room->door_y[1]+1][room->door_x[1]].symbol  == '#' ||
				(char)field[room->door_y[1]-1][room->door_x[1]].symbol  == '#') {
				room->door_x[0]++;
				room->door_x[1]++;
			}
		}
		else if (parent_room->y1 > room->y1) {
			room->door_x[0] = room->x1 + (parent_room->x2 - room->x1)/2;
			room->door_x[1] = room->x1 + (parent_room->x2 - room->x1)/2 - 1;		
			room->door_y[0] = room->y1;
			room->door_y[1] = room->y1;	
			while ((char)field[room->door_y[0]+1][room->door_x[0]].symbol  == '#' ||
				(char)field[room->door_y[0]-1][room->door_x[0]].symbol  == '#' ||
				(char)field[room->door_y[1]+1][room->door_x[1]].symbol  == '#' ||
				(char)field[room->door_y[1]-1][room->door_x[1]].symbol  == '#') {
				room->door_x[0]++;
				room->door_x[1]++;
			}
		}
		for(int i = 0; i<2; i++) {
			field[room->door_y[i]][room->door_x[i]].symbol = '/' | COLOR_PAIR(2);
		}
	} else {
		return;
	}
	div_walls_of_rooms(leaves->parent, map);
}
