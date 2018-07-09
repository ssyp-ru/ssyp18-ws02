#include <stdlib.h>
#include <math.h>
#include "rooms.h"
#include <time.h>
map_t* mapgen_tree (map_t* map, int height, int width) {
	srand (time(NULL));
	initscr();
	start_color();
	halfdelay(2);
	noecho();
	keypad(stdscr, true);
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_BLACK, COLOR_BLACK);
	init_pair(3, COLOR_YELLOW, COLOR_WHITE);

	UNPACK(field, map);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			field[j][i].symbol = '.' | COLOR_PAIR(2);
		}
	}

	room_tree_t* room_first = (room_tree_t*)malloc(sizeof(room_tree_t));
	room_first->x1 = 1;
	room_first->y1 = height-1;
	room_first->x2 = width-1;
	room_first->y2 = 4;

	create_line(room_first->x1, room_first->y1, room_first->x2, room_first->y1, '#', 1, map);
	create_line(room_first->x1, room_first->y1, room_first->x1, room_first->y2, '#', 1, map);
	create_line(room_first->x1, room_first->y2, room_first->x2, room_first->y2, '#', 1, map);
	create_line(room_first->x2, room_first->y1, room_first->x2, room_first->y2, '#', 1, map);
	
	room_first->len_x = room_first->x2 - room_first->x1;
	room_first->len_y = room_first->y1 - room_first->y2;
	room_first->S = room_first->len_x * room_first->len_y;

	tree_room_t* root = tree_create(room_first);
	vector_room_t* leaves = vector_room_init(16);
	div_room(root, leaves);
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			field[j][i].symbol = '.' | COLOR_PAIR(2);
		}
	}

	for (int i = 0; i < vector_get_len(leaves); i++) {
		create_line(vector_room_get(leaves,i)->value->x1, 
				vector_room_get(leaves,i)->value->y1, 
				vector_room_get(leaves,i)->value->x2, 
				vector_room_get(leaves,i)->value->y1, '#', 1, map);
		create_line(vector_room_get(leaves,i)->value->x1, 
				vector_room_get(leaves,i)->value->y1, 
				vector_room_get(leaves,i)->value->x1, 
				vector_room_get(leaves,i)->value->y2, '#', 1, map);
		create_line(vector_room_get(leaves,i)->value->x1, 
				vector_room_get(leaves,i)->value->y2, 
				vector_room_get(leaves,i)->value->x2, 
				vector_room_get(leaves,i)->value->y2, '#', 1, map);
		create_line(vector_room_get(leaves,i)->value->x2, 
				vector_room_get(leaves,i)->value->y1, 
				vector_room_get(leaves,i)->value->x2, 
				vector_room_get(leaves,i)->value->y2, '#', 1, map);
		div_walls_of_rooms(vector_room_get(leaves, i), map);
	}
	return map;
}

