#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "common.h"
#include "render.h"
#include "rooms.h"
#include "tree.h"
#include "dumblogger.h"
#include "vector_tree.h"
int main(int argc, char *argv[]) {
	map_t* map = (map_t*) malloc ( sizeof(map_t));
	map->height = 74;
	map->width = 200;
	map->buffer = calloc (map->height * map->width, sizeof(tile_t));
	room_t* room_first = (room_t*) malloc ( sizeof(room_t));
	srand (time(NULL));
	initscr();
	start_color();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_WHITE);
	init_pair(3, COLOR_YELLOW, COLOR_WHITE);
	UNPACK(field, map);
	for (int i = 0; i < 200; i++) {
		for (int j = 0; j < 74; j++) {
			field[j][i].symbol = '.' | COLOR_PAIR(2);
		}
	}
	room_first->x1 = rand() % 15 + 30;
	room_first->y1 = rand() % 5 + 30;
	room_first->x2 = rand() % 15 + 70;
	room_first->y2 = rand() % 5 + 10;
	create_line(room_first->x1, room_first->y1, room_first->x2, room_first->y1, '#', 1, map);
	create_line(room_first->x1, room_first->y1, room_first->x1, room_first->y2, '#', 1, map);
	create_line(room_first->x1, room_first->y2, room_first->x2, room_first->y2, '#', 1, map);
	create_line(room_first->x2, room_first->y1, room_first->x2, room_first->y2, '#', 1, map);
	room_first->len_x = room_first->x2 - room_first->x1;
	room_first->len_y = room_first->y1 - room_first->y2;
	room_first->S = room_first->len_x * room_first->len_y;
	tree_t* root = tree_create(room_first);
	vector_t* leaves = vector_init(16);
	div_room(root, leaves, map);
	for (int i = 0; i < vector_get_len(leaves); i++) {
		div_walls_of_rooms(vector_get(leaves, i), map);
	}
	render(map, 0, 0);
	refresh();
	tree_dfs(root, room_free);
	vector_free(leaves);
	tree_destroy(root);
	free(map->buffer);
	free(map);
	getch();
	endwin();
	return 0;
}
